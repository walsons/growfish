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
    QUIET,  // Non capture
    QUIET_CHECK,  // Check in quiescence search
    EVASION,
    PSEUDO_LEGAL,
    LEGAL
};

class MoveGenerator
{
public:
    friend class MovePicker;
    friend class MagicValidator;  // a class in test/magic_validator.h for test
    MoveGenerator(const Position& position) : position_(position) {}
    
    template <MoveType mt>
    std::vector<Move> GeneratePseudoLegalMoves();
    template <MoveType mt>
    std::vector<Move> GenerateLegalMoves();

    std::vector<Move> CaptureMoves() { return GenerateLegalMoves<MoveType::CAPTURE>(); }
    std::vector<Move> NonCaptureMoves() { return GenerateLegalMoves<MoveType::QUIET>(); }
    std::vector<Move> CheckMoves() { return std::vector<Move>(); }  // TODO

    Bitboard BetweenBB(Square s1, Square s2)
    {
        if (RankOf(s1) == RankOf(s2) || FileOf(s1) == FileOf(s2))  // Rook and Cannon
            return (AttackBB<PieceType::ROOK>(s1, SquareBB(s2)) & AttackBB<PieceType::ROOK>(s2, SquareBB(s1)));
        // else KNIGHT_TO
        auto r1 = RankOf(s1), r2 = RankOf(s2);
        auto c1 = FileOf(s1), c2 = FileOf(s2);
        Square barrier = s1;
        if (r2 > r1)
            barrier += SQ_NORTH;
        else
            barrier += SQ_SOUTH;
        if (c2 > c1)
            barrier += SQ_EAST;
        else
            barrier += SQ_WEST;
        return SquareBB(barrier);
    }

    Bitboard AttackerBB(Square s)
    {
        Bitboard occupancy = position_.AllPieces();
        Color c = position_.side_to_move();
        return (AttackBB<PieceType::ROOK>(s, occupancy) & position_.Pieces(c, PieceType::ROOK))
             | (AttackBB<PieceType::CANNON>(s, occupancy) & position_.Pieces(c, PieceType::CANNON))
             | (AttackBB<PieceType::KNIGHT_TO>(s, occupancy) & position_.Pieces(c, PieceType::KNIGHT))
             | (AttackBB<PieceType::BISHOP>(s, occupancy) & position_.Pieces(c, PieceType::BISHOP))
             | (AttackBB<PieceType::PAWN_TO>(s, c) & position_.Pieces(c, PieceType::PAWN))
             | (AttackBB<PieceType::ADVISOR>(s) & position_.Pieces(c, PieceType::ADVISOR))
             | (AttackBB<PieceType::KING>(s) & position_.Pieces(c, PieceType::KING));
    }

    template <Color c, PieceType pt>
    void EvasionMoves(std::vector<Move> &captureMoves, std::vector<Move> &quietMoves, Bitboard froms, Bitboard tos)
    {
        Bitboard b = position_.Pieces(c, pt);
        while (b)
        {
            Square s = PopLSB(b);

            Bitboard attack = 0, attain = 0;
            PieceMoveBB<c, pt, MoveType::PSEUDO_LEGAL>(s, attack, attain);

            attack &= position_.Pieces(!c);
            attain &= position_.Pieces(PieceType::NO_PIECE_TYPE);

            if (SquareBB(s) & froms)
            {
                while (attack)
                {
                    Square to = PopLSB(attack);
                    Move move = ConstructMove(s, to);
                    if (IsLegalMove(move))
                        captureMoves.push_back(move);
                }
                while (attain)
                {
                    Square to = PopLSB(attain);
                    Move move = ConstructMove(s, to);
                    if (IsLegalMove(move))
                        quietMoves.push_back(move);
                }
            }
            else
            {
                if ((attack = (attack & tos)))
                {
                    while (attack)
                    {
                        Square to = PopLSB(attack);
                        Move move = ConstructMove(s, to);
                        if (IsLegalMove(move))
                            captureMoves.push_back(move);
                    }
                }
                if ((attain = (attain & tos)))
                {
                    while (attain)
                    {
                        Square to = PopLSB(attain);
                        Move move = ConstructMove(s, to);
                        if (IsLegalMove(move))
                            quietMoves.push_back(move);
                    }
                }
            }
        }
    }

    void EvasionMoves(std::vector<Move> &captureMoves, std::vector<Move> &quietMoves)
    {
        Color c = position_.side_to_move();
        Square ksq = position_.KingSquare(c);
        Bitboard checkerBB = position_.CheckersBB(ksq, c, position_.AllPieces());
        // attackTo maybe the best move
        constexpr Bitboard allOnes = (Bitboard(1) << 90) - 1;
        Bitboard cannonFroms = allOnes, cannonTos = allOnes, otherTos = allOnes;
        while (checkerBB)  // rook, cannon, knight, pawn
        {
            Square s = PopLSB(checkerBB);
            if (TypeOfPiece(position_.piece_from_square(s)) == PieceType::CANNON)
            {
                cannonFroms &= BetweenBB(ksq, s);
                cannonTos &= (BetweenBB(ksq, s) | SquareBB(s));
            }
            // else if (TypeOfPiece(position_.piece_from_square(s)) == PieceType::ROOK)
            // {
            //     otherTos &= (BetweenBB(ksq, s) | SquareBB(s));
            // }
            // else if (TypeOfPiece(position_.piece_from_square(s)) == PieceType::KNIGHT)
            // {
            //     otherTos &= (BetweenBB(ksq, s) | SquareBB(s));
            // }
            // else if (TypeOfPiece(position_.piece_from_square(s)) == PieceType::PAWN)
            // {
            //     otherTos &= (BetweenBB(ksq, s) | SquareBB(s));
            // }
            else
                otherTos &= (BetweenBB(ksq, s) | SquareBB(s));
            // other moves are king move
        }
        cannonFroms &= position_.Pieces(position_.side_to_move());
        Bitboard froms = 0, tos = 0;
        if (otherTos != allOnes)
        {
            tos = otherTos;
        }
        else if (cannonFroms != allOnes)
        {
            froms = cannonFroms;
            tos = cannonTos;
        }

        constexpr PieceType pts[] = {PieceType::ROOK, PieceType::CANNON,
                           PieceType::KNIGHT, PieceType::PAWN,
                           PieceType::BISHOP, PieceType::ADVISOR,
                           PieceType::KING};
        if (c == Color::RED)
        {
            EvasionMoves<Color::RED, pts[0]>(captureMoves, quietMoves, froms, tos);
            EvasionMoves<Color::RED, pts[1]>(captureMoves, quietMoves, froms, tos);
            EvasionMoves<Color::RED, pts[2]>(captureMoves, quietMoves, froms, tos);
            EvasionMoves<Color::RED, pts[3]>(captureMoves, quietMoves, froms, tos);
            EvasionMoves<Color::RED, pts[4]>(captureMoves, quietMoves, froms, tos);
            EvasionMoves<Color::RED, pts[5]>(captureMoves, quietMoves, froms, tos);
            // EvasionMoves<Color::RED, pts[6]>(captureMoves, quietMoves, froms, tos);
        }
        else
        {
            EvasionMoves<Color::BLACK, pts[0]>(captureMoves, quietMoves, froms, tos);
            EvasionMoves<Color::BLACK, pts[1]>(captureMoves, quietMoves, froms, tos);
            EvasionMoves<Color::BLACK, pts[2]>(captureMoves, quietMoves, froms, tos);
            EvasionMoves<Color::BLACK, pts[3]>(captureMoves, quietMoves, froms, tos);
            EvasionMoves<Color::BLACK, pts[4]>(captureMoves, quietMoves, froms, tos);
            EvasionMoves<Color::BLACK, pts[5]>(captureMoves, quietMoves, froms, tos);
            // EvasionMoves<Color::BLACK, pts[6]>(captureMoves, quietMoves, froms, tos);
        }
        
        Bitboard b = PieceMove<MoveType::PSEUDO_LEGAL>(ksq);
        while (b)
        {
            Square to = PopLSB(b);
            auto move = ConstructMove(ksq, to);
            if (IsLegalMove(move))
            {
                if (position_.piece_from_square(to) == Piece::NO_PIECE)
                    quietMoves.push_back(move);
                else
                    captureMoves.push_back(move);
            }
        }
    }

    bool IsLegalMove(Move move);

    template <Color c, PieceType pt, MoveType mt>
    Bitboard PieceMove(Square s)
    {
        Bitboard attack = 0, attain = 0;
        PieceMoveBB<c, pt, mt>(s, attack, attain);
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
        PieceMoveBB<c, pt, mt>(s, attack, attain);
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
    void AddMove(const Position& position, std::vector<Move>& moves)
    {
        Bitboard b = position.Pieces(c, pt);
        while (b)
        {
            Square s = PopLSB(b);
            PieceMove<c, pt, mt>(s, moves);
        }
    }

    template <Color c, PieceType pt, MoveType mt>
    void PieceMoveBB(Square s, Bitboard &attack, Bitboard &attain)
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
            attack = AttackBB<pt>(s);
        }
        else if constexpr (pt == PieceType::PAWN)
        {
            attack = AttackBB<PieceType::PAWN>(s, c);
        }
        else
        {
            occupies = ~position_.Pieces(PieceType::NO_PIECE_TYPE);
            attack = AttackBB<pt>(s, occupies);
        }

        if constexpr (pt == PieceType::CANNON)
            attain = AttackBB<PieceType::ROOK>(s, occupies);
        else
            attain = attack;
    }

private:
    Position position_;
};

template <MoveType mt>
std::vector<Move> MoveGenerator::GeneratePseudoLegalMoves()
{
    static_assert(mt == MoveType::CAPTURE || mt == MoveType::QUIET || mt == MoveType::PSEUDO_LEGAL);

    std::vector<Move> pseudoLegalMoves;
    if (position_.side_to_move() == Color::RED)
    {
        AddMove<Color::RED, PieceType::ROOK, mt>(position_, pseudoLegalMoves);
        AddMove<Color::RED, PieceType::CANNON, mt>(position_, pseudoLegalMoves);
        AddMove<Color::RED, PieceType::KNIGHT, mt>(position_, pseudoLegalMoves);
        AddMove<Color::RED, PieceType::PAWN, mt>(position_, pseudoLegalMoves);
        AddMove<Color::RED, PieceType::BISHOP, mt>(position_, pseudoLegalMoves);
        AddMove<Color::RED, PieceType::ADVISOR, mt>(position_, pseudoLegalMoves);
        AddMove<Color::RED, PieceType::KING, mt>(position_, pseudoLegalMoves);
    }
    else
    {
        AddMove<Color::BLACK, PieceType::ROOK, mt>(position_, pseudoLegalMoves);
        AddMove<Color::BLACK, PieceType::CANNON, mt>(position_, pseudoLegalMoves);
        AddMove<Color::BLACK, PieceType::KNIGHT, mt>(position_, pseudoLegalMoves);
        AddMove<Color::BLACK, PieceType::PAWN, mt>(position_, pseudoLegalMoves);
        AddMove<Color::BLACK, PieceType::BISHOP, mt>(position_, pseudoLegalMoves);
        AddMove<Color::BLACK, PieceType::ADVISOR, mt>(position_, pseudoLegalMoves);
        AddMove<Color::BLACK, PieceType::KING, mt>(position_, pseudoLegalMoves);
    }

    return pseudoLegalMoves;
}

template <MoveType mt>
std::vector<Move> MoveGenerator::GenerateLegalMoves()
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

#endif