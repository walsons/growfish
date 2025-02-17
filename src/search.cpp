#include <iostream>
#include <algorithm>

#include "search.h"
#include "move_generator.h"
#include "evaluate.h"
#include "transposition_table.h"
#include "history.h"
#include "move_picker.h"
#include "book.h"
#include "thread_pool.h"

extern TranspositionTable TT;
extern ThreadPool THREAD_POOL;
extern History HISTORIES[THREAD_NUM];
extern Book BOOK;

std::atomic<unsigned long long> Search::search_nodes = 0;

void Search::Start(const SearchCondition &sc)
{
    // First check if there is a move in the book
    if (search_book(root_position_.GenerateFen()))
        return;

    max_search_time_ = sc.time;
    IterativeDeepeningLoop(sc.depth);
}

void Search::IterativeDeepeningLoop(Depth maxDepth)
{
    Search::search_nodes = 1;

    generate_root_moves();
    if (root_moves_.empty()) 
    {
        best_move_ = 0;
        best_score_ = -MateValue;
        return;
    }

    TT.NewSearch();
    for (auto& history: HISTORIES) { history.Clear(); }
    // Ensure best_move_ is valid when search is aborted very fast
    best_move_ = root_moves_.front().move;
    best_score_ = -Infinite;
    abort_search_ = false;
    start_timestamp_ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    // start other threads
    for (size_t threadIndex = 1; threadIndex < THREAD_NUM; ++threadIndex)
    {
        THREAD_POOL.Submit(&Search::thread_root_search, this, (maxDepth / 2 * 2), new SearchStack[100], threadIndex, root_position_, root_moves_);
    }

    SearchStack ss[100];
    // Disable odd depth
    for (Depth depth = 2; depth <= maxDepth; depth += 2)
    {
        root_search(depth, ss, 0);
    }

    if (print_pv_move_)
    {
        std::cout << "pv move: ";
        for (auto pvMove: ss[0].pv)
        {
            std::cout << pvMove << " ";
        }
        std::cout << std::endl;
    }
}

bool Search::search_book(std::string fen)
{
    bool hitBook = false;
    assert(fen.size() > 8);
    fen = fen.substr(0, fen.size() - 8);
    Move bookMove = BOOK.SearchBestMove(fen);
    if (bookMove != 0)
    {
        best_move_ = bookMove;
        best_score_ = 12345;  // A special value to indicate the move is from book
        hitBook = true;
    }
    return hitBook;
}

void Search::generate_root_moves()
{
    root_moves_.clear();
    MoveGenerator moveGenerator(root_position_);
    auto legalmoves = moveGenerator.GenerateLegalMoves<MoveType::LEGAL>();
    for (auto move: legalmoves)
    {
        UndoInfo undoInfo;
        root_position_.MakeMove(move, undoInfo);
        if (root_position_.ExistsInPast(root_position_.key()))
        {
            // two case for prohibit move(maybe more): 
            // 1. check enemy king (continuously check)
            // 2. our moving piece is rook (continuously capture)
            if (root_position_.IsChecked(root_position_.side_to_move()) || TypeOfPiece(root_position_.piece_from_square(MoveTo(move))) == PieceType::ROOK)
            {
                root_position_.UndoMove(undoInfo);
                continue;
            }
        }
        // Note qsearch is minus
        root_moves_.emplace_back(move, -qsearch(root_position_, -Infinite, Infinite, new SearchStack[100], 0, 0));
        root_position_.UndoMove(undoInfo);
    }
    root_moves_.sort([](const RootMove& a, const RootMove& b) { return a.score > b.score; });
}

void Search::root_search(Depth depth, SearchStack ss[], size_t threadIndex)
{
    ++Search::search_nodes;
    Value cur_ply_best_move = 0;
    Value cur_ply_best_score = -Infinite;
    if (depth <= 0) { return; }

    Value alpha = -Infinite, beta = Infinite;
    Ply ply = 0;
    int moveCount = 0;
    for (auto [move, _]: root_moves_)
    {
        UndoInfo undoInfo;
        Position position = root_position_;
        position.MakeMove(move, undoInfo);
        Value score = 0;
        if (moveCount++ == 0)
            score = -search(position, depth - 1, -beta, -alpha, ss, ply + 1, threadIndex);
        else
        {
            score = -search_nonpv(position, depth - 1, -alpha - 1, -alpha, ss, ply + 1, threadIndex);
            if (score > alpha)
                score = -search(position, depth - 1, -beta, -alpha, ss, ply + 1, threadIndex);
        }
        position.UndoMove(undoInfo);
        if (score > alpha)
        {
            cur_ply_best_move = move;
            cur_ply_best_score = score;
            alpha = score;
            ss[ply].current_move = move;
            ss[ply].pv.clear();
            ss[ply].pv.push_back(ss[ply].current_move);
            ss[ply].pv.insert(ss[ply].pv.end(), std::make_move_iterator(ss[ply + 1].pv.begin()), 
                                                std::make_move_iterator(ss[ply + 1].pv.end()));
        }
    }

    if (!abort_search_ && cur_ply_best_move != 0)
    {
        best_move_ = cur_ply_best_move;
        best_score_ = cur_ply_best_score;
        // I have no idea why this way is a little faster than direct update score and sort
        for (auto it = root_moves_.begin(); it != root_moves_.end(); ++it)
        {
            if (it->move == best_move_)
            {
                root_moves_.erase(it);
                break;
            }
        }
        root_moves_.emplace_front(best_move_, best_score_);
    }
}

void Search::thread_root_search(Depth depth, SearchStack ss[], size_t threadIndex, Position rootPosition, std::list<RootMove> rootMoves)
{
    ++Search::search_nodes;
    if (depth <= 0) { return; }

    Value alpha = -Infinite, beta = Infinite;
    Ply ply = 0;
    int moveCount = 0;
    for (auto [move, _]: rootMoves)
    {
        UndoInfo undoInfo;
        Position position = rootPosition;
        position.MakeMove(move, undoInfo);
        Value score = 0;
        if (moveCount++ == 0)
            score = -search(position, depth - 1, -beta, -alpha, ss, ply + 1, threadIndex);
        else
        {
            score = -search_nonpv(position, depth - 1, -alpha - 1, -alpha, ss, ply + 1, threadIndex);
            if (score > alpha)
                score = -search(position, depth - 1, -beta, -alpha, ss, ply + 1, threadIndex);
        }
        position.UndoMove(undoInfo);
        if (score > alpha)
        {
            alpha = score;
            ss[ply].current_move = move;
            ss[ply].pv.clear();
            ss[ply].pv.push_back(ss[ply].current_move);
            ss[ply].pv.insert(ss[ply].pv.end(), std::make_move_iterator(ss[ply + 1].pv.begin()), 
                                                std::make_move_iterator(ss[ply + 1].pv.end()));
        }
    }
    // if (alpha != -Infinite)
    // {
    //     for (auto it = rootMoves.begin(); it != rootMoves.end(); ++it)
    //     {
    //         if (it->move == ss[ply].current_move)
    //         {
    //             rootMoves.erase(it);
    //             rootMoves.emplace_front(ss[ply].current_move, alpha);
    //             break;
    //         }
    //     }
    // }
}

Value Search::search(Position& position, Depth depth, Value alpha, Value beta, SearchStack ss[], Ply ply, size_t threadIndex)
{
    ++Search::search_nodes;

    // In PV search, we only use transposition table to retrieve best move for MovePicker,
    // otherwise it would lead to bad results
    TTEntry ttEntry = TT[position.key()];
    MovePicker movePicker(threadIndex, position, ttEntry.key != 0 ? ttEntry.move : Move(), ss[ply].killer_move);
    Move move = movePicker.NextMove();
    if (move == 0) 
    {
        Value score = -MateValue + ply;
        TT.Store(position.key(), TT.AdjustSetValue(score, ply), depth, 0, ValueType::EXACT);
        // We like choose fastest checkmate in search
        return score;
    }

    if (depth == 0)
    {
        auto score = qsearch(position, alpha, beta, ss, ply + 1, threadIndex);
        return score;
    }

    // Mate distance pruning
    if (alpha >= MateValue - ply - 1)
        return alpha;
    if (-MateValue + ply >= beta)
        return -MateValue + ply;

    Value oldAlpha = alpha;
    int moveCount = 0;
    while (move && !abort_search_)
    {
        UndoInfo undoInfo;
        position.MakeMove(move, undoInfo);
        Value score = 0;
        if (moveCount++ == 0)
            score = -search(position, depth - 1, -beta, -alpha, ss, ply + 1, threadIndex);
        else
        {
            score = -search_nonpv(position, depth - 1, -alpha - 1, -alpha, ss, ply + 1, threadIndex);
            if (score > alpha)
                score = -search(position, depth - 1, -beta, -alpha, ss, ply + 1, threadIndex);
        }
        position.UndoMove(undoInfo);
        if (score >= beta)
        {
            TT.Store(position.key(), TT.AdjustSetValue(score, ply), depth, move, ValueType::LOWER_BOUND);
            // Only non capture move do history and as killer move 
            if (position.piece_from_square(move.MoveTo()) == Piece::NO_PIECE)
            {
                // HISTORY.Success(position, move, depth);
                HISTORIES[threadIndex].Success(position, move, depth);
                if (ss[ply].killer_move[0] != move)
                {
                    ss[ply].killer_move[1] = ss[ply].killer_move[0];
                    ss[ply].killer_move[0] = move;
                }
            }
            return score;
        }
        else if (score > alpha)
        {
            TT.Store(position.key(), TT.AdjustSetValue(score, ply), depth, move, ValueType::EXACT);
            alpha = score;
            ss[ply].current_move = move;
            ss[ply].pv.clear();
            ss[ply].pv.push_back(ss[ply].current_move);
            ss[ply].pv.insert(ss[ply].pv.end(), std::make_move_iterator(ss[ply + 1].pv.begin()), 
                                                std::make_move_iterator(ss[ply + 1].pv.end()));
        }
        move = movePicker.NextMove();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - start_timestamp_ > max_search_time_)
        {
            abort_search_ = true;
        }
    }
    // if alpha is not updated, it's a upperbound of this node
    if (oldAlpha == alpha)
        TT.Store(position.key(), TT.AdjustSetValue(alpha, ply), depth, 0, ValueType::UPPER_BOUND);
    
    return alpha;
}

Value Search::search_nonpv(Position& position, Depth depth, Value alpha, Value beta, SearchStack ss[], Ply ply, size_t threadIndex)
{
    ++Search::search_nodes;

    TTEntry ttEntry = TT[position.key()];
    if (ttEntry.key != 0 && TT.CanUseTT(ttEntry, depth, ply, beta))
    {
        return TT.AdjustGetValue(ttEntry.value, ply);
    }
    MovePicker movePicker(threadIndex, position, ttEntry.key != 0 ? ttEntry.move : Move(), ss[ply].killer_move);

    Move move = movePicker.NextMove();
    if (move == 0) 
    {
        Value score = -MateValue + ply;
        TT.Store(position.key(), TT.AdjustSetValue(score, ply), depth, 0, ValueType::EXACT);
        // We like choose fastest checkmate in search
        return score;
    }

    if (depth == 0)
    {
        auto score = qsearch(position, alpha, beta, ss, ply + 1, threadIndex);
        return score;
    }

    // Mate distance pruning
    if (alpha >= MateValue - ply - 1)
        return alpha;
    if (-MateValue + ply >= beta)
        return -MateValue + ply;

    // razoring
    if (depth <= 2)
    {
        constexpr Value razorMargin = 400;
        Value score = Evaluate::Eval(position);
        if (score < beta - razorMargin)
        {
            score = qsearch(position, alpha, beta, ss, ply + 1, threadIndex);
            if (score < beta)
                return score;
        }
    }

    Value oldAlpha = alpha;
    while (move && !abort_search_)
    {
        UndoInfo undoInfo;
        position.MakeMove(move, undoInfo);
        Value score = -search_nonpv(position, depth - 1, -beta, -alpha, ss, ply + 1, threadIndex);
        position.UndoMove(undoInfo);
        if (score >= beta)
        {
            TT.Store(position.key(), TT.AdjustSetValue(score, ply), depth, move, ValueType::LOWER_BOUND);
            // Only non capture move do history and as killer move 
            if (position.piece_from_square(move.MoveTo()) == Piece::NO_PIECE)
            {
                // HISTORY.Success(position, move, depth);
                HISTORIES[threadIndex].Success(position, move, depth);
                if (ss[ply].killer_move[0] != move)
                {
                    ss[ply].killer_move[1] = ss[ply].killer_move[0];
                    ss[ply].killer_move[0] = move;
                }
            }
            return score;
        }
        else if (score > alpha)
        {
            TT.Store(position.key(), TT.AdjustSetValue(score, ply), depth, move, ValueType::EXACT);
            alpha = score;
            ss[ply].current_move = move;
            ss[ply].pv.clear();
            ss[ply].pv.push_back(ss[ply].current_move);
            ss[ply].pv.insert(ss[ply].pv.end(), std::make_move_iterator(ss[ply + 1].pv.begin()), 
                                                std::make_move_iterator(ss[ply + 1].pv.end()));
        }
        move = movePicker.NextMove();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - start_timestamp_ > max_search_time_)
        {
            abort_search_ = true;
        }
    }
    // if alpha is not updated, it's a upperbound of this node
    if (oldAlpha == alpha)
        TT.Store(position.key(), TT.AdjustSetValue(alpha, ply), depth, 0, ValueType::UPPER_BOUND);
    
    return alpha;
}

Value Search::qsearch(Position& position, Value alpha, Value beta, SearchStack ss[], Ply ply, size_t threadIndex)
{
    ++Search::search_nodes;

    TTEntry ttEntry = TT[position.key()];
    if (ttEntry.key != 0 && TT.CanUseTT(ttEntry, 0, ply, beta))
    {
        return TT.AdjustGetValue(ttEntry.value, ply);
    }

    Value score = Evaluate::Eval(position);
    if (score >= beta)
    {
        return score;
    }
    else if (score > alpha)
    {
        alpha = score;
    }
    Move killerMove[2] = {0, 0};
    if (MovePicker(threadIndex, position, 0, killerMove).NextMove() == 0)
    {
        // we can't store this value to transposition due to this case only consider capture moves
        return -MateValue + ply;
    }
    MovePicker movePicker(threadIndex, position, 0, killerMove, MovePicker::Phase::QSEARCH_CAPTURE);
    Move move = movePicker.NextMove();
    while (move && !abort_search_)
    {
        UndoInfo undoInfo;
        position.MakeMove(move, undoInfo);
        score = -qsearch(position, -beta, -alpha, ss, ply + 1, threadIndex);
        position.UndoMove(undoInfo);
        if (score >= beta)
        {
            return beta;
        }
        else if (score > alpha)
        {
            alpha = score;
        }
        move = movePicker.NextMove();
    }
    return alpha;
}
