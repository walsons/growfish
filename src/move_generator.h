#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <vector>

#include "bitboard.h"
#include "position.h"
#include "function.h"
#include "magic.h"

enum class MoveType : char
{
    CAPTURE,
    QUIET,
    QUIET_CHECK,
    EVASION,
    PSEUDO_LEGAL,
    LEGAL
};

class MoveGenerator
{
public:
    friend class MovePicker;
    friend class MagicValidator;  // a class in test/magic_validator.h for test
    MoveGenerator(const Position& position) : position_(position)
    {
    }
    
    std::vector<Move> CaptureMoves() 
    {
        return GenerateLegalMoves<MoveType::CAPTURE>();
    }

    std::vector<Move> NonCaptureMoves() 
    {
        return GenerateLegalMoves<MoveType::QUIET>();
    }

    std::vector<Move> CheckMoves() { return std::vector<Move>(); }  // TODO

    template <MoveType mt>
    std::vector<Move> GeneratePseudoLegalMoves()
    {
        static_assert(mt == MoveType::CAPTURE || mt == MoveType::QUIET || mt == MoveType::PSEUDO_LEGAL);

        std::vector<Move> pseudoLegalMoves;
        for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST)
        {
            if (position_.piece_from_square(s) == Piece::NO_PIECE || position_.color_from_square(s) != position_.side_to_move())
                continue;
            
            switch (position_.piece_from_square(s))
            {
            case Piece::W_ROOK:
                PieceMove<Color::RED, PieceType::ROOK, mt>(s, pseudoLegalMoves);
                break;
            case Piece::B_ROOK:
                PieceMove<Color::BLACK, PieceType::ROOK, mt>(s, pseudoLegalMoves);
                break;
            case Piece::W_KNIGHT:
                PieceMove<Color::RED, PieceType::KNIGHT, mt>(s, pseudoLegalMoves);
                break;
            case Piece::B_KNIGHT:
                PieceMove<Color::BLACK, PieceType::KNIGHT, mt>(s, pseudoLegalMoves);
                break;
            case Piece::W_BISHOP:
                PieceMove<Color::RED, PieceType::BISHOP, mt>(s, pseudoLegalMoves);
                break;
            case Piece::B_BISHOP:
                PieceMove<Color::BLACK, PieceType::BISHOP, mt>(s, pseudoLegalMoves);
                break;
            case Piece::W_ADVISOR:
                PieceMove<Color::RED, PieceType::ADVISOR, mt>(s, pseudoLegalMoves);
                break;
            case Piece::B_ADVISOR:
                PieceMove<Color::BLACK, PieceType::ADVISOR, mt>(s, pseudoLegalMoves);
                break;
            case Piece::W_KING:
                PieceMove<Color::RED, PieceType::KING, mt>(s, pseudoLegalMoves);
                break;
            case Piece::B_KING:
                PieceMove<Color::BLACK, PieceType::KING, mt>(s, pseudoLegalMoves);
                break;
            case Piece::W_CANNON:
                PieceMove<Color::RED, PieceType::CANNON, mt>(s, pseudoLegalMoves);
                break;
            case Piece::B_CANNON:
                PieceMove<Color::BLACK, PieceType::CANNON, mt>(s, pseudoLegalMoves);
                break;
            case Piece::W_PAWN:
                PieceMove<Color::RED, PieceType::PAWN, mt>(s, pseudoLegalMoves);
                break;
            case Piece::B_PAWN:
                PieceMove<Color::BLACK, PieceType::PAWN, mt>(s, pseudoLegalMoves);
                break;
            default:
                break;
            }
        }

        return pseudoLegalMoves;
    }

    template <MoveType mt>
    std::vector<Move> GenerateLegalMoves()
    {
        static_assert(mt == MoveType::CAPTURE || mt == MoveType::QUIET || mt == MoveType::LEGAL);

        std::vector<Move> pseudoLegalMoves;
        if constexpr (mt == MoveType::LEGAL)
            pseudoLegalMoves  = GeneratePseudoLegalMoves<MoveType::PSEUDO_LEGAL>();
        else
            pseudoLegalMoves = GeneratePseudoLegalMoves<mt>();

        std::vector<Move> legalMoves;
        for (Move move: pseudoLegalMoves)
        {
            if (IsLegalMove(move))
                legalMoves.push_back(move);
        }
        return legalMoves;
    }

    bool IsLegalMove(Move move)
    {
        Color c = position_.side_to_move();
        auto kPos = position_.KingSquare(c);
        int kRow = kPos / 9;
        int kCol = kPos % 9;
        if (!position_.IsSelfChecked())
        {
            auto from = MoveFrom(move);
            auto to = MoveTo(move);
            if (from != kPos)
            {
                if (from / 9 != kRow && from % 9 != kCol && to / 9 != kRow && to % 9 != kCol && Distance(from, kPos) > 1)
                {
                    return true;
                }
                else
                {
                    UndoInfo undoInfo;
                    position_.SimpleMakeMove(move, undoInfo);
                    if (!position_.IsSelfChecked())
                    {
                        position_.SimpleUndoMove(undoInfo);
                        return true;
                    }
                    position_.SimpleUndoMove(undoInfo);
                }
            }
            else
            {
                UndoInfo undoInfo;
                position_.SimpleMakeMove(move, undoInfo);
                if (!position_.IsSelfChecked())
                {
                    position_.SimpleUndoMove(undoInfo);
                    return true;
                }
                position_.SimpleUndoMove(undoInfo);
            }
        }
        else
        {
            auto from = MoveFrom(move);
            auto to = MoveTo(move);
            if (from != kPos)
            {
                if (from / 9 != kRow && from % 9 != kCol && to / 9 != kRow && to % 9 != kCol && Distance(to, kPos) > 2)
                {
                    return false;
                }
                else
                {
                    UndoInfo undoInfo;
                    position_.SimpleMakeMove(move, undoInfo);
                    if (!position_.IsSelfChecked())
                    {
                        position_.SimpleUndoMove(undoInfo);
                        return true;
                    }
                    position_.SimpleUndoMove(undoInfo);
                }
            }
            else
            {
                UndoInfo undoInfo;
                position_.SimpleMakeMove(move, undoInfo);
                if (!position_.IsSelfChecked())
                {
                    position_.SimpleUndoMove(undoInfo);
                    return true;
                }
                position_.SimpleUndoMove(undoInfo);
            }
        }
        return false;
    }

    template <Color c, PieceType pt, MoveType mt>
    Bitboard PieceMove(Square s)
    {
        Bitboard attack = 0, attain = 0;
        PieceMoveBitboard<c, pt, mt>(s, attack, attain);
        attack &= position_.Pieces(!c);
        attain &= position_.Pieces(PieceType::NO_PIECE_TYPE);
        return attack | attain;
    }
    // runtime version
    template <MoveType mt>
    Bitboard PieceMove(Square s)
    {
        switch (position_.piece_from_square(s))
        {
        case Piece::W_ROOK:
            return PieceMove<Color::RED, PieceType::ROOK, mt>(s);
        case Piece::B_ROOK:
            return PieceMove<Color::BLACK, PieceType::ROOK, mt>(s);
        case Piece::W_KNIGHT:
            return PieceMove<Color::RED, PieceType::KNIGHT, mt>(s);
        case Piece::B_KNIGHT:
            return PieceMove<Color::BLACK, PieceType::KNIGHT, mt>(s);
        case Piece::W_BISHOP:
            return PieceMove<Color::RED, PieceType::BISHOP, mt>(s);
        case Piece::B_BISHOP:
            return PieceMove<Color::BLACK, PieceType::BISHOP, mt>(s);
        case Piece::W_ADVISOR:
            return PieceMove<Color::RED, PieceType::ADVISOR, mt>(s);
        case Piece::B_ADVISOR:
            return PieceMove<Color::BLACK, PieceType::ADVISOR, mt>(s);
        case Piece::W_KING:
            return PieceMove<Color::RED, PieceType::KING, mt>(s);
        case Piece::B_KING:
            return PieceMove<Color::BLACK, PieceType::KING, mt>(s);
        case Piece::W_CANNON:
            return PieceMove<Color::RED, PieceType::CANNON, mt>(s);
        case Piece::B_CANNON:
            return PieceMove<Color::BLACK, PieceType::CANNON, mt>(s);
        case Piece::W_PAWN:
            return PieceMove<Color::RED, PieceType::PAWN, mt>(s);
        case Piece::B_PAWN:
            return PieceMove<Color::BLACK, PieceType::PAWN, mt>(s);
        default:
            break;
        }
        return 0;
    }


    template <Color c, PieceType pt, MoveType mt>
    void PieceMove(Square s, std::vector<Move> &moves)
    {
        Bitboard attack = 0, attain = 0;
        PieceMoveBitboard<c, pt, mt>(s, attack, attain);
        if constexpr (mt == MoveType::CAPTURE || mt == MoveType::PSEUDO_LEGAL)
        {
            attack &= position_.Pieces(!c);
            while (attack)
            {
                Square to = PopLSB(attack);
                moves.push_back(ConstructMove(s, to));
            }
        }
        if constexpr (mt == MoveType::QUIET || mt == MoveType::PSEUDO_LEGAL)
        {
            attain &= position_.Pieces(PieceType::NO_PIECE_TYPE);
            while (attain)
            {
                Square to = PopLSB(attain);
                moves.push_back(ConstructMove(s, to));
            }
        }
    }
    // runtime version
    template <MoveType mt>
    void PieceMove(Square s, std::vector<Move> &moves)
    {
        std::vector<Move> pseudoLegalMoves;
        switch (position_.piece_from_square(s))
        {
        case Piece::W_ROOK:
            PieceMove<Color::RED, PieceType::ROOK, mt>(s, pseudoLegalMoves);
            break;
        case Piece::B_ROOK:
            PieceMove<Color::BLACK, PieceType::ROOK, mt>(s, pseudoLegalMoves);
            break;
        case Piece::W_KNIGHT:
            PieceMove<Color::RED, PieceType::KNIGHT, mt>(s, pseudoLegalMoves);
            break;
        case Piece::B_KNIGHT:
            PieceMove<Color::BLACK, PieceType::KNIGHT, mt>(s, pseudoLegalMoves);
            break;
        case Piece::W_BISHOP:
            PieceMove<Color::RED, PieceType::BISHOP, mt>(s, pseudoLegalMoves);
            break;
        case Piece::B_BISHOP:
            PieceMove<Color::BLACK, PieceType::BISHOP, mt>(s, pseudoLegalMoves);
            break;
        case Piece::W_ADVISOR:
            PieceMove<Color::RED, PieceType::ADVISOR, mt>(s, pseudoLegalMoves);
            break;
        case Piece::B_ADVISOR:
            PieceMove<Color::BLACK, PieceType::ADVISOR, mt>(s, pseudoLegalMoves);
            break;
        case Piece::W_KING:
            PieceMove<Color::RED, PieceType::KING, mt>(s, pseudoLegalMoves);
            break;
        case Piece::B_KING:
            PieceMove<Color::BLACK, PieceType::KING, mt>(s, pseudoLegalMoves);
            break;
        case Piece::W_CANNON:
            PieceMove<Color::RED, PieceType::CANNON, mt>(s, pseudoLegalMoves);
            break;
        case Piece::B_CANNON:
            PieceMove<Color::BLACK, PieceType::CANNON, mt>(s, pseudoLegalMoves);
            break;
        case Piece::W_PAWN:
            PieceMove<Color::RED, PieceType::PAWN, mt>(s, pseudoLegalMoves);
            break;
        case Piece::B_PAWN:
            PieceMove<Color::BLACK, PieceType::PAWN, mt>(s, pseudoLegalMoves);
            break;
        default:
            break;
        }
    }

private:
    template <Color c, PieceType pt, MoveType mt>
    void PieceMoveBitboard(Square s, Bitboard &attack, Bitboard &attain)
    {
        static_assert( pt == PieceType::ROOK 
            || pt == PieceType::CANNON
            || pt == PieceType::KNIGHT
            || pt == PieceType::BISHOP
            || pt == PieceType::ADVISOR
            || pt == PieceType::KING
            || pt == PieceType::PAWN
            , "Unsupported piece type"
        );
        static_assert(mt == MoveType::CAPTURE || mt == MoveType::QUIET || mt == MoveType::PSEUDO_LEGAL);

        attack = 0;
        attain = 0;
        Bitboard occupies = 0;

        if constexpr (pt == PieceType::ADVISOR
                        || pt == PieceType::KING)
        {
            attack = Attack<pt>(s);
        }
        else if constexpr (pt == PieceType::PAWN)
        {
            attack = Attack<PieceType::PAWN, c>(s);
        }
        else
        {
            occupies = ~position_.Pieces(PieceType::NO_PIECE_TYPE);
            attack = Attack<pt>(occupies, s);
        }

        if constexpr (pt == PieceType::CANNON)
            attain = Attack<PieceType::ROOK>(occupies, s);
        else
            attain = attack;
    }

    /*
    template <Color c>
    void RookMove(Square s)
    {
        Bitboard occupies = ~position_.Pieces(PieceType::NO_PIECE_TYPE);
        Bitboard attack = Attack<PieceType::ROOK>(occupies, s);
        Bitboard attain = attack;
        attack &= position_.Pieces(!c);
        while (attack)
        {
            Square to = PopLSB(attack);
            pseudo_legal_capture_moves_.push_back(ConstructMove(s, to));
        }
        attain &= position_.Pieces(PieceType::NO_PIECE_TYPE);
        while (attain)
        {
            Square to = PopLSB(attain);
            pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, to));
        }
    }

    template <Color c>
    void CannonMove(Square s)
    {
        Bitboard occupies = ~position_.Pieces(PieceType::NO_PIECE_TYPE);
        Bitboard attack = Attack<PieceType::CANNON>(occupies, s);
        Bitboard attain = Attack<PieceType::ROOK>(occupies, s);
        attack &= position_.Pieces(!c);
        while (attack)
        {
            Square to = PopLSB(attack);
            pseudo_legal_capture_moves_.push_back(ConstructMove(s, to));
        }
        attain &= position_.Pieces(PieceType::NO_PIECE_TYPE);
        while (attain)
        {
            Square to = PopLSB(attain);
            pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, to));
        }
    }

    template <Color c>
    void KnightMove(Square s)
    {
        Bitboard occupies = ~position_.Pieces(PieceType::NO_PIECE_TYPE);
        Bitboard attack = Attack<PieceType::KNIGHT>(occupies, s);
        Bitboard attain = attack;
        attack &= position_.Pieces(!c);
        while (attack)
        {
            Square to = PopLSB(attack);
            pseudo_legal_capture_moves_.push_back(ConstructMove(s, to));
        }
        attain &= position_.Pieces(PieceType::NO_PIECE_TYPE);
        while (attain)
        {
            Square to = PopLSB(attain);
            pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, to));
        }
    }

    template <Color c>
    void BishopMove(Square s)
    {
        Bitboard occupies = ~position_.Pieces(PieceType::NO_PIECE_TYPE);
        Bitboard attack = Attack<PieceType::BISHOP>(occupies, s);
        Bitboard attain = attack;
        attack &= position_.Pieces(!c);
        while (attack)
        {
            Square to = PopLSB(attack);
            pseudo_legal_capture_moves_.push_back(ConstructMove(s, to));
        }
        attain &= position_.Pieces(PieceType::NO_PIECE_TYPE);
        while (attain)
        {
            Square to = PopLSB(attain);
            pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, to));
        }
    }

    template <Color c>
    void AdvisorMove(Square s)
    {
        Bitboard attack = Attack<PieceType::ADVISOR>(s);
        Bitboard attain = attack;
        attack &= position_.Pieces(!c);
        while (attack)
        {
            Square to = PopLSB(attack);
            pseudo_legal_capture_moves_.push_back(ConstructMove(s, to));
        }
        attain &= position_.Pieces(PieceType::NO_PIECE_TYPE);
        while (attain)
        {
            Square to = PopLSB(attain);
            pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, to));
        }
    }

    template <Color c>
    void KingMove(Square s)
    {
        Bitboard attack = Attack<PieceType::KING>(s);
        Bitboard attain = attack;
        attack &= position_.Pieces(!c);
        while (attack)
        {
            Square to = PopLSB(attack);
            pseudo_legal_capture_moves_.push_back(ConstructMove(s, to));
        }
        attain &= position_.Pieces(PieceType::NO_PIECE_TYPE);
        while (attain)
        {
            Square to = PopLSB(attain);
            pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, to));
        }
        // Special case: face enemy King
        auto usKingBB = position_.Pieces(c, PieceType::KING);
        auto enemyKingBB = position_.Pieces(!c, PieceType::KING);
        File usKingFile = FileOf(PopLSB(usKingBB));
        File enemyKingFile = FileOf(PopLSB(enemyKingBB));
        usKingBB = position_.Pieces(c, PieceType::KING);
        enemyKingBB = position_.Pieces(!c, PieceType::KING);
        if (usKingFile == enemyKingFile && ((usKingBB | enemyKingBB) == (FileBB(usKingFile) & ~position_.Pieces(PieceType::NO_PIECE_TYPE))))
        {
            Square to = PopLSB(enemyKingBB);
            pseudo_legal_capture_moves_.push_back(ConstructMove(s, to));
        }
    }

    template <Color c>
    void PawnMove(Square s)
    {
        Bitboard attack = Attack<PieceType::PAWN, c>(s);
        Bitboard attain = attack;
        attack &= position_.Pieces(!c);
        while (attack)
        {
            Square to = PopLSB(attack);
            pseudo_legal_capture_moves_.push_back(ConstructMove(s, to));
        }
        attain &= position_.Pieces(PieceType::NO_PIECE_TYPE);
        while (attain)
        {
            Square to = PopLSB(attain);
            pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, to));
        }
    }
    */

    // Moves that don't consider being checked
    void GeneratePseudoLegalMoves();
    // Available moves
    void GenerateLegalMoves();

private:
    Position position_;

    // std::vector<Move> pseudo_legal_capture_moves_;
    // std::vector<Move> pseudo_legal_non_capture_moves_;

    // std::vector<Move> capture_moves_;
    // std::vector<Move> non_capture_moves_;
    // Check moves are from both capture moves and non_capture moves
    // std::vector<Move> check_moves_;
};

#endif