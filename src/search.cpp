#include <iostream>
#include <algorithm>

#include "search.h"
#include "move_generator.h"
#include "evaluate.h"
#include "transposition_table.h"
#include "history.h"
#include "move_picker.h"
#include "book.h"

extern TranspositionTable TT;
extern History HISTORY;
extern Book BOOK;

unsigned long long Search::search_nodes = 0;

void Search::IterativeDeepeningLoop(int maxDepth)
{
    Search::search_nodes = 1;  // root node
    TT.NewSearch();
    HISTORY.Clear();

    root_moves_.clear();
    Move emptyKillerMove[2] = {0, 0};
    MovePicker rootMovePicker(root_position_, 0, emptyKillerMove);
    Move move = rootMovePicker.NextMove();
    while (move)
    {
        root_moves_.push_back(move);
        move = rootMovePicker.NextMove();
    }
    // MoveGenerator moveGenerator(root_position_);
    // auto checkMoves = moveGenerator.CheckMoves();
    // root_moves_.insert(root_moves_.end(), std::make_move_iterator(checkMoves.begin()), std::make_move_iterator(checkMoves.end()));
    // auto captureMoves = moveGenerator.CaptureMoves();
    // root_moves_.insert(root_moves_.end(), std::make_move_iterator(captureMoves.begin()), std::make_move_iterator(captureMoves.end()));
    // auto nonCaptureMoves = moveGenerator.NonCaptureMoves();
    // root_moves_.insert(root_moves_.end(), std::make_move_iterator(nonCaptureMoves.begin()), std::make_move_iterator(nonCaptureMoves.end()));

    auto fen = root_position_.GenerateFen();
    assert(fen.size() > 8);
    fen = fen.substr(0, fen.size() - 8);
    Move bookMove = BOOK.SearchBestMove(fen);
    if (bookMove != 0)
    {
        best_move_ = bookMove;
        best_score_ = 1234;
        return;
    }

    int depth = 2;
    if (maxDepth % 2)  // maxDepth is an odd number
    {
        depth = 1;
    }
    for (; depth <= maxDepth; depth += 2)
    {
        SearchStack ss[100];
        root_search(depth, ss);
        
        if (ss[0].pv.size() > 0)
        {
            root_moves_.remove(ss[0].pv[0]);
            root_moves_.push_front(ss[0].pv[0]);
        }

        // for (auto move : root_moves_)
        // {
        //     std::cout << move << "; ";
        // }
        // std::cout << std::endl;

        // std::cout << "depth: " << depth << " pv: ";
        // for (auto move: ss[0].pv)
        // {
        //     std::cout << move << "; ";
        // }
        // std::cout << std::endl;
    }
}

void Search::root_search(int depth, SearchStack ss[])
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

    int alpha = -Infinite, beta = Infinite;
    int ply = 0;
    for (auto move: root_moves_)
    {
        UndoInfo undoInfo;
        root_position_.MakeMove(move, undoInfo);
        int score = -search(root_position_, depth - 1, -beta, -alpha, ss, ply + 1);
        root_position_.UndoMove(undoInfo);
        if (score > alpha)
        {
            best_move_ = move;
            best_score_ = score;
            alpha = score;
            ss[ply].current_move = move;
            ss[ply].pv.clear();
            ss[ply].pv.push_back(ss[ply].current_move);
            ss[ply].pv.insert(ss[ply].pv.end(), ss[ply + 1].pv.begin(), ss[ply + 1].pv.end());
        }
    }
}

int Search::search(Position& position, int depth, int alpha, int beta, SearchStack ss[], int ply)
{
    ++Search::search_nodes;
    TTEntry* ttEntry = TT[position.key()];
    if (ttEntry != nullptr && TT.CanUseTT(ttEntry, depth, ply, beta))
    {
        return TT.AdjustGetValue(ttEntry->value, ply);
    }

    MovePicker movePicker(position, ttEntry != nullptr ? ttEntry->move : Move(), ss[ply].killer_move);
    Move move = movePicker.NextMove();
    if (move == 0) 
    {
        auto score = -MateValue + ply;
        TT.Store(position.key(), TT.AdjustSetValue(score, ply), depth, 0, ValueType::EXACT);
        // We like choose fastest checkmate in search
        return score;
    }

    if (depth == 0)
    {
        auto score = qsearch(position, alpha, beta, ss, ply + 1);
        return score;
    }

    // Mate distance pruning
    alpha = std::max(alpha, -MateValue + ply);
    beta = std::min(beta, MateValue - ply - 1);
    if (alpha >= beta)
        return alpha;

    // int oldAlpha = alpha;
    while (move)
    {
        UndoInfo undoInfo;
        position.MakeMove(move, undoInfo);
        auto score = -search(position, depth - 1, -beta, -alpha, ss, ply + 1);
        position.UndoMove(undoInfo);
        if (score >= beta)
        {
            TT.Store(position.key(), TT.AdjustSetValue(score, ply), depth, move, ValueType::LOWER_BOUND);
            // Only non capture move do history and as killer move 
            if (position.piece_from_square(move.MoveTo()) == Piece::NO_PIECE)
            {
                HISTORY.Success(position, move, depth);
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
            alpha = score;
            ss[ply].current_move = move;
            ss[ply].pv.clear();
            ss[ply].pv.push_back(ss[ply].current_move);
            ss[ply].pv.insert(ss[ply].pv.end(), ss[ply + 1].pv.begin(), ss[ply + 1].pv.end());
            TT.Store(position.key(), TT.AdjustSetValue(score, ply), depth, move, ValueType::EXACT);
        }
        move = movePicker.NextMove();
    }
    // if alpha is not updated, it's a upperbound of this node
    // if (oldAlpha == alpha)
    //     TT.Store(position.key(), TT.AdjustSetValue(alpha, ply), depth, 0, ValueType::UPPER_BOUND);
    
    return alpha;
}

int Search::qsearch(Position& position, int alpha, int beta, SearchStack ss[], int ply)
{
    ++Search::search_nodes;
    auto score = Evaluate::Eval(position);
    if (score >= beta)
    {
        return score;
    }
    else if (score > alpha)
    {
        alpha = score;
    }
    Move killerMove[2] = {0, 0};
    if (MovePicker(position, 0, killerMove).NextMove() == 0)
    {
        auto score = -MateValue + ply;
        TT.Store(position.key(), TT.AdjustSetValue(score, ply), 0, 0, ValueType::EXACT);
        return -MateValue + ply;
    }
    MovePicker movePicker(position, 0, killerMove, MovePicker::Phase::QSEARCH_CAPTURE);
    Move move = movePicker.NextMove();
    while (move)
    {
        UndoInfo undoInfo;
        position.MakeMove(move, undoInfo);
        score = -qsearch(position, -beta, -alpha, ss, ply + 1);
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
