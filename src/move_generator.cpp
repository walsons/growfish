#include "move_generator.h"

void MoveGenerator::GeneratePseudoLegalMoves()
{
    Color c = position_.side_to_move();
    for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST)
    {
        if (position_.color_from_square(s) != c)
            continue;
        
        switch (position_.piece_from_square(s))
        {
        case Piece::W_ROOK:
            PieceMove<Color::RED, PieceType::ROOK, MoveType::PSEUDO_LEGAL>(s);
            // RookMove<Color::RED>(s);
            break;
        case Piece::B_ROOK:
            PieceMove<Color::BLACK, PieceType::ROOK, MoveType::PSEUDO_LEGAL>(s);
            // RookMove<Color::BLACK>(s);
            break;
        case Piece::W_KNIGHT:
            PieceMove<Color::RED, PieceType::KNIGHT, MoveType::PSEUDO_LEGAL>(s);
            // KnightMove<Color::RED>(s);
            break;
        case Piece::B_KNIGHT:
            PieceMove<Color::BLACK, PieceType::KNIGHT, MoveType::PSEUDO_LEGAL>(s);
            // KnightMove<Color::BLACK>(s);
            break;
        case Piece::W_BISHOP:
            PieceMove<Color::RED, PieceType::BISHOP, MoveType::PSEUDO_LEGAL>(s);
            // BishopMove<Color::RED>(s);
            break;
        case Piece::B_BISHOP:
            PieceMove<Color::BLACK, PieceType::BISHOP, MoveType::PSEUDO_LEGAL>(s);
            // BishopMove<Color::BLACK>(s);
            break;
        case Piece::W_ADVISOR:
            PieceMove<Color::RED, PieceType::ADVISOR, MoveType::PSEUDO_LEGAL>(s);
            // AdvisorMove<Color::RED>(s);
            break;
        case Piece::B_ADVISOR:
            PieceMove<Color::BLACK, PieceType::ADVISOR, MoveType::PSEUDO_LEGAL>(s);
            // AdvisorMove<Color::BLACK>(s);
            break;
        case Piece::W_KING:
            PieceMove<Color::RED, PieceType::KING, MoveType::PSEUDO_LEGAL>(s);
            // KingMove<Color::RED>(s);
            break;
        case Piece::B_KING:
            PieceMove<Color::BLACK, PieceType::KING, MoveType::PSEUDO_LEGAL>(s);
            // KingMove<Color::BLACK>(s);
            break;
        case Piece::W_CANNON:
            PieceMove<Color::RED, PieceType::CANNON, MoveType::PSEUDO_LEGAL>(s);
            // CannonMove<Color::RED>(s);
            break;
        case Piece::B_CANNON:
            PieceMove<Color::BLACK, PieceType::CANNON, MoveType::PSEUDO_LEGAL>(s);
            // CannonMove<Color::BLACK>(s);
            break;
        case Piece::W_PAWN:
            PieceMove<Color::RED, PieceType::PAWN, MoveType::PSEUDO_LEGAL>(s);
            // PawnMove<Color::RED>(s);
            break;
        case Piece::B_PAWN:
            PieceMove<Color::BLACK, PieceType::PAWN, MoveType::PSEUDO_LEGAL>(s);
            // PawnMove<Color::BLACK>(s);
            break;
        default:
            break;
        }
    }
}

void MoveGenerator::GenerateLegalMoves()
{
    Color c = position_.side_to_move();
    auto kPos = position_.KingSquare(c);
    int kRow = kPos / 9;
    int kCol = kPos % 9;

    GeneratePseudoLegalMoves();

    auto movesFilter = [&](std::vector<Move>& pseudoLegalMoves, std::vector<Move>& legalMoves) {
        if (!position_.IsSelfChecked())
        {
            for (auto pMove : pseudoLegalMoves)
            {
                auto from = MoveFrom(pMove);
                auto to = MoveTo(pMove);
                if (from != kPos)
                {
                    if (from / 9 != kRow && from % 9 != kCol && to / 9 != kRow && to % 9 != kCol && Distance(from, kPos) > 1)
                    {
                        // TODO: optimize this funciton, due to this case will probably has check move
                        legalMoves.push_back(pMove);
                    }
                    else
                    {
                        UndoInfo undoInfo;
                        position_.SimpleMakeMove(pMove, undoInfo);
                        if (!position_.IsSelfChecked())
                        {
                            if (position_.IsEnemyChecked())
                                check_moves_.push_back(pMove);
                            else
                                legalMoves.push_back(pMove);
                        }
                        position_.SimpleUndoMove(undoInfo);
                    }
                }
                else
                {
                    UndoInfo undoInfo;
                    position_.SimpleMakeMove(pMove, undoInfo);
                    if (!position_.IsSelfChecked())
                    {
                        if (position_.IsEnemyChecked())
                            check_moves_.push_back(pMove);
                        else
                            legalMoves.push_back(pMove);
                    }
                    position_.SimpleUndoMove(undoInfo);
                }
            }
        }
        else
        {
            for (auto pMove : pseudoLegalMoves)
            {
                auto from = MoveFrom(pMove);
                auto to = MoveTo(pMove);
                if (from != kPos)
                {
                    if (from / 9 != kRow && from % 9 != kCol && to / 9 != kRow && to % 9 != kCol && Distance(to, kPos) > 2)
                    {
                        continue;
                    }
                    else
                    {
                        UndoInfo undoInfo;
                        position_.SimpleMakeMove(pMove, undoInfo);
                        if (!position_.IsSelfChecked())
                        {
                            if (position_.IsEnemyChecked())
                                check_moves_.push_back(pMove);
                            else
                                legalMoves.push_back(pMove);
                        }
                        position_.SimpleUndoMove(undoInfo);
                    }
                }
                else
                {
                    UndoInfo undoInfo;
                    position_.SimpleMakeMove(pMove, undoInfo);
                    if (!position_.IsSelfChecked())
                    {
                        if (position_.IsEnemyChecked())
                            check_moves_.push_back(pMove);
                        else
                            legalMoves.push_back(pMove);
                    }
                    position_.SimpleUndoMove(undoInfo);
                }
            }
        }
    };
    
    movesFilter(pseudo_legal_capture_moves_, capture_moves_);
    movesFilter(pseudo_legal_non_capture_moves_, non_capture_moves_);
}