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
extern size_t THREAD_NUM;
extern ThreadPool THREAD_POOL;
std::vector<History> HISTORIES(THREAD_NUM);
extern Book BOOK;

std::atomic<unsigned long long> Search::search_nodes = 0;

void Search::IterativeDeepeningLoop(Depth maxDepth)
{
    Search::search_nodes = 1;  // root node
    TT.NewSearch();
    for (auto& history: HISTORIES)
    {
        history.Clear();
    }

    root_moves_.clear();
    Move emptyKillerMove[2] = {0, 0};
    MovePicker rootMovePicker(0, root_position_, 0, emptyKillerMove);
    Move move = rootMovePicker.NextMove();
    while (move)
    {
        root_moves_.push_back(move);
        move = rootMovePicker.NextMove();
    }

    auto fen = root_position_.GenerateFen();
    assert(fen.size() > 8);
    fen = fen.substr(0, fen.size() - 8);
    Move bookMove = BOOK.SearchBestMove(fen);
    if (bookMove != 0)
    {
        best_move_ = bookMove;
        best_score_ = 12345;
        return;
    }

    // start other threads
    for (size_t threadIndex = 1; threadIndex < THREAD_NUM; ++threadIndex)
    {
        THREAD_POOL.submit(&Search::thread_root_search, this, (maxDepth / 2 * 2), new SearchStack[100], threadIndex, root_position_, root_moves_);
    }

    SearchStack ss[100];
    // Disable odd depth
    for (Depth depth = 2; depth <= maxDepth; depth += 2)
    {
        root_search(depth, ss, 0);
        
        if (ss[0].pv.size() > 0)
        {
            root_moves_.remove(ss[0].pv[0]);
            root_moves_.push_front(ss[0].pv[0]);
        }
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

void Search::root_search(Depth depth, SearchStack ss[], size_t threadIndex)
{
    ++Search::search_nodes;
    best_move_ = 0;
    best_score_ = -Infinite;
    prohibited_move_ = 0;
    if (depth <= 0) { return; }

    if (root_moves_.empty()) 
    {
        best_score_ = -MateValue;
        return;
    }

    Value alpha = -Infinite, beta = Infinite;
    Ply ply = 0;
    for (auto move: root_moves_)
    {
        UndoInfo undoInfo;
        Position position = root_position_;
        position.MakeMove(move, undoInfo);
        Value score = -search(position, depth - 1, -beta, -alpha, ss, ply + 1, threadIndex);
        position.UndoMove(undoInfo);
        if (score > alpha)
        {
            best_move_ = move;
            best_score_ = score;
            alpha = score;
            ss[ply].current_move = move;
            ss[ply].pv.clear();
            ss[ply].pv.push_back(ss[ply].current_move);
            ss[ply].pv.insert(ss[ply].pv.end(), std::make_move_iterator(ss[ply + 1].pv.begin()), 
                                                std::make_move_iterator(ss[ply + 1].pv.end()));
        }
    }
}

void Search::thread_root_search(Depth depth, SearchStack ss[], size_t threadIndex, Position rootPosition, std::list<Move> rootMoves)
{
    ++Search::search_nodes;
    if (depth <= 0) { return; }

    if (rootMoves.empty()) 
    {
        return;
    }

    Value alpha = -Infinite, beta = Infinite;
    Ply ply = 0;
    for (auto move: rootMoves)
    {
        UndoInfo undoInfo;
        Position position = rootPosition;
        position.MakeMove(move, undoInfo);
        Value score = -search(position, depth - 1, -beta, -alpha, ss, ply + 1, threadIndex);
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
}

Value Search::search(Position& position, Depth depth, Value alpha, Value beta, SearchStack ss[], Ply ply, size_t threadIndex)
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

    Value oldAlpha = alpha;
    while (move)
    {
        UndoInfo undoInfo;
        position.MakeMove(move, undoInfo);
        Value score = -search(position, depth - 1, -beta, -alpha, ss, ply + 1, threadIndex);
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
    }
    // if alpha is not updated, it's a upperbound of this node
    if (oldAlpha == alpha)
        TT.Store(position.key(), TT.AdjustSetValue(alpha, ply), depth, 0, ValueType::UPPER_BOUND);
    
    return alpha;
}

Value Search::qsearch(Position& position, Value alpha, Value beta, SearchStack ss[], Ply ply, size_t threadIndex)
{
    ++Search::search_nodes;
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
    while (move)
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
