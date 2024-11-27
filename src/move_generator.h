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
        // rook:17 cannon:17 knight:8 bishop:4 advisor:4 king:4 pawn:3
        // 17 + 17 + 8 + 4 + 4 + 4 + 3 = 57
        pseudo_legal_capture_moves_.reserve(57);
        pseudo_legal_non_capture_moves_.reserve(57);

        capture_moves_.reserve(57);
        non_capture_moves_.reserve(57);
        check_moves_.reserve(57);

        GenerateLegalMoves();
    }
    
    std::vector<Move> capture_moves() { return capture_moves_; }
    std::vector<Move> non_capture_moves() { return non_capture_moves_; }
    std::vector<Move> check_moves() { return check_moves_; }

    // target is the bitboard of piece destination
    template <Color c, PieceType pt, MoveType mt>
    std::vector<Move> GenerateMoves(Bitboard target)
    {
        static_assert(pt != PieceType::KING, "This method doesn't support king");

        Bitboard bb = position_.Pieces(c, pt);
        while (bb)
        {
            Square from = PopLSB(bb);
        }
        // TODO
        return std::vector<Move>();
    }

private:
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

    // Moves that don't consider being checked
    void GeneratePseudoLegalMoves();
    // Available moves
    void GenerateLegalMoves();

private:
    Position position_;
    
    std::vector<Move> pseudo_legal_capture_moves_;
    std::vector<Move> pseudo_legal_non_capture_moves_;

    std::vector<Move> capture_moves_;
    std::vector<Move> non_capture_moves_;
    // Check moves are from both capture moves and non_capture moves
    std::vector<Move> check_moves_;
};

#endif