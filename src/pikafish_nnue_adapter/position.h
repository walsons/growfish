/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2022 The Stockfish developers (see AUTHORS file)

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED

#include <cassert>
#include <deque>
#include <memory> // For std::unique_ptr
#include <string>
#include <stack>

#include "bitboard.h"
#include "types.h"

#include "nnue/nnue_accumulator.h"

namespace Stockfish {

/// StateInfo struct stores information needed to restore a Position object to
/// its previous state when we retract a move. Whenever a move is made on the
/// board (by calling Position::do_move), a StateInfo object must be passed.
struct StateInfo {
    StateInfo* previous;
    Piece      capturedPiece;

    // Used by NNUE
    Eval::NNUE::Accumulator accumulator;
    DirtyPiece dirtyPiece;
};


/// A list to keep track of the position states along the setup moves (from the
/// start position to the position just before the search starts). Needed by
/// 'draw by repetition' detection. Use a std::deque because pointers to
/// elements are not invalidated upon list resizing.
typedef std::unique_ptr<std::deque<StateInfo>> StateListPtr;


/// Position class stores information regarding the board representation as
/// pieces, side to move, hash keys, etc. Important methods are
/// do_move() and undo_move(), used by the search to update node info when
/// traversing the search tree.
class Position {
public:
    Position() = default;
    Position(const Position&) = delete;
    Position& operator=(const Position&) = delete;

    // FEN string input/output
    Position& set(const std::string& fenStr);
    Position& set(const Position& pos);
    std::string fen() const;

    // Position representation
    Bitboard pieces(PieceType pt) const;
    Bitboard pieces(PieceType pt1, PieceType pt2) const;
    Bitboard pieces(Color c) const;
    Bitboard pieces(Color c, PieceType pt) const;
    Bitboard pieces(Color c, PieceType pt1, PieceType pt2) const;
    Bitboard pieces(Color c, PieceType pt1, PieceType pt2, PieceType pt3) const;
    Piece piece_on(Square s) const;
    bool empty(Square s) const;
    template<PieceType Pt> int count(Color c) const;
    template<PieceType Pt> int count() const;
    template<PieceType Pt> Square square(Color c) const;

    bool capture(Move m) const;
    Piece moved_piece(Move m) const;
    Piece captured_piece() const;

    // Doing and undoing moves
    void do_move(Move m);
    void do_move(Move m, StateInfo& newSt);
    void undo_move(Move m);
    void do_null_move();
    void do_null_move(StateInfo& newSt);
    void undo_null_move();


    // Other properties of the position
    Color side_to_move() const;
    bool not_only_pawn(Color c) const;

    // Used by NNUE
    StateInfo* state() const;

    void put_piece(Piece pc, Square s);
    void remove_piece(Square s);

private:
    // Other helpers
    void move_piece(Square from, Square to);

    // Data members
    Piece board[SQUARE_NB];
    Bitboard byTypeBB[PIECE_TYPE_NB];
    Bitboard byColorBB[COLOR_NB];
    int pieceCount[PIECE_NB];
    StateInfo* st;
    int gamePly;
    Color sideToMove;

    // Add a state in do_move and remove it in undo_move
    StateListPtr states;
};

extern std::ostream& operator<<(std::ostream& os, const Position& pos);

inline Color Position::side_to_move() const {
    return sideToMove;
}

inline Piece Position::piece_on(Square s) const {
    assert(is_ok(s));
    return board[s];
}

inline bool Position::empty(Square s) const {
    return piece_on(s) == NO_PIECE;
}

inline Piece Position::moved_piece(Move m) const {
    return piece_on(from_sq(m));
}

inline Bitboard Position::pieces(PieceType pt = ALL_PIECES) const {
    return byTypeBB[pt];
}

inline Bitboard Position::pieces(PieceType pt1, PieceType pt2) const {
    return pieces(pt1) | pieces(pt2);
}

inline Bitboard Position::pieces(Color c) const {
    return byColorBB[c];
}

inline Bitboard Position::pieces(Color c, PieceType pt) const {
    return pieces(c) & pieces(pt);
}

inline Bitboard Position::pieces(Color c, PieceType pt1, PieceType pt2) const {
    return pieces(c) & (pieces(pt1) | pieces(pt2));
}

inline Bitboard Position::pieces(Color c, PieceType pt1, PieceType pt2, PieceType pt3) const {
    return pieces(c) & (pieces(pt1) | pieces(pt2) | pieces(pt3));
}

template<PieceType Pt> inline int Position::count(Color c) const {
    return pieceCount[make_piece(c, Pt)];
}

template<PieceType Pt> inline int Position::count() const {
    return count<Pt>(WHITE) + count<Pt>(BLACK);
}

template<PieceType Pt> inline Square Position::square(Color c) const {
    assert(count<Pt>(c) == 1);
    return lsb(pieces(c, Pt));
}

inline bool Position::not_only_pawn(Color c) const {
    return count<ALL_PIECES>(c) - count<PAWN>(c);
}

inline bool Position::capture(Move m) const {
    assert(is_ok(m));
    // Castling is encoded as "king captures rook"
    return !empty(to_sq(m));
}

inline Piece Position::captured_piece() const {
    return st->capturedPiece;
}

inline void Position::put_piece(Piece pc, Square s) {
    board[s] = pc;
    byTypeBB[ALL_PIECES] |= byTypeBB[type_of(pc)] |= s;
    byColorBB[color_of(pc)] |= s;
    pieceCount[pc]++;
    pieceCount[make_piece(color_of(pc), ALL_PIECES)]++;
}

inline void Position::remove_piece(Square s) {
    Piece pc = board[s];
    byTypeBB[ALL_PIECES] ^= s;
    byTypeBB[type_of(pc)] ^= s;
    byColorBB[color_of(pc)] ^= s;
    board[s] = NO_PIECE;
    pieceCount[pc]--;
    pieceCount[make_piece(color_of(pc), ALL_PIECES)]--;
}

inline void Position::move_piece(Square from, Square to) {
    Piece pc = board[from];
    Bitboard fromTo = from | to;
    byTypeBB[ALL_PIECES] ^= fromTo;
    byTypeBB[type_of(pc)] ^= fromTo;
    byColorBB[color_of(pc)] ^= fromTo;
    board[from] = NO_PIECE;
    board[to] = pc;
}

inline StateInfo* Position::state() const {
    return st;
}

inline Position& Position::set(const Position& pos) {
    set(pos.fen());
    return *this;
}

} // namespace Stockfish

#endif // #ifndef POSITION_H_INCLUDED
