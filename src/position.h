#ifndef POSITION_H
#define POSITION_H

#include <cstring>

#include <string>
#include <stack>

#include "types.h"
#include "function.h"
#include "zobrist.h"

const std::string kStartPos = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1";

class Board
{
public:
    Board() : b(new Piece[90]) {}
    ~Board() { delete[] b; }
    Board(const Board& board) : b(new Piece[90]) { std::memcpy(b, board.b, 90 * sizeof(Piece)); }
    Board& operator=(const Board& board)
    {
        // self assignment
        if (board.b == b)
            return *this;
        std::memcpy(b, board.b, 90 * sizeof(Piece));
        return *this;
    }

    Piece& operator[](Square index) { return b[index]; }
    const Piece& operator[](Square index) const { return b[index]; }
private:
    Piece* b;  // board representation
};

class Position
{
public:
    Position(const std::string& fen = kStartPos) { SetPosition(fen); }

    void MakeMove(Move move, UndoInfo& undoInfo);
    void UndoMove(const UndoInfo& undoInfo);

    void SetPosition(const std::string& fen);
    void ResetPosition() { SetPosition(kStartPos); }
    std::string GenerateFen() const;
    void DisplayBoard(bool reverse = false) const;

    Square KingSquare(Color c) const;
    bool IsEnemyChecked();
    bool IsSelfChecked();

    Bitboard CheckersBB(Square ksq, Color kc, Bitboard occupancy) const;
    bool IsLegalMove(Move move) const;

    Piece piece_from_square(Square s) const { return board_[s]; }
    Color color_from_square(Square s) const 
    { 
        assert((IsColor<Color::RED>(board_[s]) || IsColor<Color::BLACK>(board_[s])));
        return IsColor<Color::RED>(board_[s]) ? Color::RED : Color::BLACK; 
    }
    U64 key() const { return key_; }
    Color side_to_move() const { return side_to_move_; }

    int piece_count() const { return 90 - piece_count_[NUM(Piece::NO_PIECE)]; }

    template <typename... PieceTypes>
    Bitboard Pieces(PieceType pt, PieceTypes... pts) const
    {
        return (by_type_bb_[NUM(pt)] | ... | by_type_bb_[NUM(pts)]);
    }
    Bitboard Pieces(Color c) const
    {
        return by_color_bb_[NUM(c)];
    }
    template <typename... PieceTypes>
    Bitboard Pieces(Color c, PieceTypes... pts) const
    {
        return Pieces(c) & Pieces(pts...);
    }
    Bitboard AllPieces() const
    {
        return by_color_bb_[NUM(Color::RED)] | by_color_bb_[NUM(Color::BLACK)];
    }

private:
    bool IsChecked(Color c);
    Piece PieceFromChar(char c) const;
    char CharFromPiece(Piece piece) const;

    void put_piece(Piece p, Square s);
    void remove_piece(Square s);
    void move_piece(Square from, Square to);

private:
    Board board_;
    U64 key_;
    Color side_to_move_;

    // Bitboard
    Bitboard by_type_bb_[NUM(PieceType::PIECE_TYPE_NUM)];
    Bitboard by_color_bb_[NUM(Color::COLOR_NUM)];
    int piece_count_[NUM(Piece::PIECE_NUM)];
};

#endif