#include "move_picker.h"
#include <algorithm>

extern History HISTORY;

MovePicker::MovePicker(const Position& position, Move ttMove, Move killerMove[], Phase phase)
    : position_(position)
    , tt_move_(ttMove)
    , killer_move1_(killerMove[0])
    , killer_move2_(killerMove[1])
    , move_generator_(position)
    , phase_(phase)
{
    // ensure tt_move and killer_move is valid(existed and legal)
    auto moveValid = [&](Move move) {
        auto from = move.MoveFrom(), to = move.MoveTo();
        if (SquareBB(from) & position_.Pieces(position_.side_to_move()))
        {
            Bitboard b = move_generator_.PieceMove<MoveType::PSEUDO_LEGAL>(from);
            if  (SquareBB(to) & b)
            {
                return move_generator_.IsLegalMove(move);
            }
        }
        return false;
    };

    if (tt_move_ != 0 && !moveValid(tt_move_))
        tt_move_ = 0;
    if (killer_move1_ != 0 && !moveValid(killer_move1_))
        killer_move1_ = 0;
    if (killer_move2_ != 0 && !moveValid(killer_move2_))
        killer_move2_ = 0;

    if (position_.IsChecked(position_.side_to_move()) && phase_ != Phase::QSEARCH_CAPTURE)
    {
        phase_ = Phase::EVASION;
        // std::list<ScoreMove> evasionList;
        // if (phase_ == Phase::QSEARCH_CAPTURE)
        // {
        //     evasionList = GenerateEvasionMove(1);
        // }
        // else
        // {
        //     evasionList = GenerateEvasionMove();
        // }
        // for (auto item : evasionList)
        // {
        //     evasions_.push_back(item);
        // }
        // std::stable_sort(evasions_.begin(), evasions_.end(), [](ScoreMove a, ScoreMove b) {
        //     return a.score > b.score;
        // });
        // evasions_index_ = 0;
    }
}
    
std::list<ScoreMove> MovePicker::GenerateCheckMove()
{
    std::list<ScoreMove> moves;
    // TODO
    return moves;
}

std::list<ScoreMove> MovePicker::GenerateCaptureMove()
{
    bad_captures_.clear();
    std::list<ScoreMove> moves;
    auto captureMoves = move_generator_.GenerateLegalMoves<MoveType::CAPTURE>();
    Color c = position_.side_to_move();
    for (auto move : captureMoves)
    {
        int score = (c == Color::RED ? SEECapture<Color::RED>(move) : SEECapture<Color::BLACK>(move));
        if (score >= 0)
        {
            int score = MVV_LVA[PieceIndex(position_.piece_from_square(move.MoveFrom()))][PieceIndex(position_.piece_from_square(move.MoveTo()))];
            moves.push_back({ move, score });
        }
        else
            bad_captures_.push_back({ move, score });
    }
    return moves;
}

std::list<ScoreMove> MovePicker::GenerateCaptureMove_only_mvv_lva()
{
    std::list<ScoreMove> moves;
    auto captureMoves = move_generator_.GenerateLegalMoves<MoveType::CAPTURE>();
    for (auto move : captureMoves)
    {
        int score = MVV_LVA[PieceIndex(position_.piece_from_square(move.MoveFrom()))][PieceIndex(position_.piece_from_square(move.MoveTo()))];
        moves.push_back({ move, score });
    }
    return moves;
}

std::list<ScoreMove> MovePicker::GenerateNonCaptureMove()
{
    std::list<ScoreMove> moves;
    auto nonCaptureMoves = move_generator_.GenerateLegalMoves<MoveType::QUIET>();
    for (auto move : nonCaptureMoves)
    {
        int score = HISTORY.HistoryValue(position_, move);
        moves.push_back({ move, score });
    }
    return moves;
}

std::list<ScoreMove> MovePicker::GenerateEvasionMove(int a)
{
    std::list<ScoreMove> moves;
    std::vector<Move> captures, quiets;
    if (tt_move_ != 0)
    {
        moves.push_back({tt_move_, 3 * History::kHistoryMax});
    }
    move_generator_.EvasionMoves(captures, quiets);
    for (auto move : captures)
    {
        int score = MVV_LVA[PieceIndex(position_.piece_from_square(move.MoveFrom()))][PieceIndex(position_.piece_from_square(move.MoveTo()))];
        moves.push_back({ move, score + 2 * History::kHistoryMax });
    }

    if (a == 1)
        return moves;
    if (killer_move1_ != 0)
        moves.push_back({killer_move1_, History::kHistoryMax + 1000});
    if (killer_move2_ != 0)
        moves.push_back({killer_move2_, History::kHistoryMax + 500});
    for (auto move : quiets)
    {
        int score = HISTORY.HistoryValue(position_, move);
        moves.push_back({ move, score });
    }

    return moves;

}

Move MovePicker::NextMove()
{
    // if (position_.IsChecked(position_.side_to_move()))
    // {
    //     position_.DisplayBoard();
    //     std::cout << position_.GenerateFen() << std::endl;
    // }

    Move selectedMove;
    bool loop = true;

    while (selectedMove == 0 && loop)
    {
        if (!moves_.empty())
        {
            auto bestMoveIter = moves_.begin();
            auto iter = moves_.begin();
            ++iter;
            for (; iter != moves_.end(); ++iter)
            {
                if (iter->score > bestMoveIter->score)
                {
                    bestMoveIter = iter;
                }
            }
            selectedMove = bestMoveIter->move;
            moves_.erase(bestMoveIter);
        }
        else
        {
            switch (phase_)
            {
            case Phase::TT:
            {
                if (tt_move_ != 0)
                    moves_ = { ScoreMove{tt_move_, 0} };
                phase_ = Phase::CHECK;
            }
            break;
            case Phase::CHECK:
            {
                moves_ = GenerateCheckMove();
                phase_ = Phase::CAPTURE;
            }
            break;
            case Phase::CAPTURE:
            {
                // currently GenerateCaptureMove_only_mvv_lva is faster,
                // however, GenerateCaptureMove() search less node
                moves_ = GenerateCaptureMove_only_mvv_lva();
                // moves_ = GenerateCaptureMove();
                phase_ = Phase::KILLER;
            }
            break;
            case Phase::KILLER:
            {
                if (killer_move1_ != 0)
                    moves_.push_back(ScoreMove{killer_move1_, 0});
                if (killer_move2_ != 0)
                    moves_.push_back(ScoreMove{killer_move2_, 0});
                phase_ = Phase::NON_CAPTURE;
            }
            break;
            case Phase::NON_CAPTURE:
            {
                moves_ = GenerateNonCaptureMove();
                phase_ = Phase::BAD_CAPTURE;
            }
            break;
            case Phase::BAD_CAPTURE:
            {
                moves_ = bad_captures_;
                phase_ = Phase::END;
            }
            break;
            case Phase::EVASION:
            {
                moves_ = GenerateEvasionMove();
                phase_ = Phase::END;
            }
            break;
            case Phase::QSEARCH_CAPTURE:
            {
                moves_ = GenerateCaptureMove();
                phase_ = Phase::END;
            }
            break;
            case Phase::END:
            {
                loop = false;
            }
            break;
            }
        }
    }

    // if (position_.IsChecked(position_.side_to_move()) && selectedMove != 0)
    // {
    //     // std::cout << selectedMove << "  " << evasions_[evasions_index_].move << std::endl;
    //     if ((selectedMove != evasions_[evasions_index_].move))
    //     {
    //         bool found = false;
    //         for (auto item : evasions_)
    //         {
    //             if (item.move == selectedMove)
    //             {
    //                 found = true;
    //                 break;
    //             }
    //         }
    //         if (!found)
    //         {
    //             position_.DisplayBoard();
    //             GenerateEvasionMove();
    //         }
    //         assert(found);
    //     }
    //     evasions_index_++;
    // }
    return selectedMove;
}
