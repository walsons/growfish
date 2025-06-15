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

#include <algorithm>
#include <cassert>
#include <cstddef> // For offsetof()
#include <cstring> // For std::memset, std::memcmp
#include <iomanip>
#include <sstream>

#include "bitboard.h"
#include "misc.h"
#include "position.h"
#include "uci.h"

using std::string;

namespace Stockfish {

namespace Zobrist {

Key psq[PIECE_NB][SQUARE_NB];
Key side;

} // namespace Zobrist

namespace {

const string PieceToChar(" RACPNBK racpnbk");

constexpr Piece Pieces[] = { W_ROOK, W_ADVISOR, W_CANNON, W_PAWN, W_KNIGHT, W_BISHOP, W_KING,
                             B_ROOK, B_ADVISOR, B_CANNON, B_PAWN, B_KNIGHT, B_BISHOP, B_KING };
} // anonymous namespace


/// operator<<(Position) returns an ASCII representation of the position
std::ostream& operator<<(std::ostream& os, const Position& pos) {
    os << "\n +---+---+---+---+---+---+---+---+---+\n";

    for (Rank r = RANK_9; r >= RANK_0; --r)
    {
        for (File f = FILE_A; f <= FILE_I; ++f)
            os << " | " << PieceToChar[pos.piece_on(make_square(f, r))];

        os << " | " << r << "\n +---+---+---+---+---+---+---+---+---+\n";
    }

    os << "   a   b   c   d   e   f   g   h   i\n"
       << "\nFen: " << pos.fen() << "\nKey: " << std::hex << std::uppercase
       << std::setfill('0') << std::setw(16)
       << std::setfill(' ') << std::dec << "\n";

    return os;
}


/// Position::set() initializes the position object with the given FEN string.
/// This function is not very robust - make sure that input FENs are correct,
/// this is assumed to be the responsibility of the GUI.
Position& Position::set(const string& fenStr) {
/*
   A FEN string defines a particular position using only the ASCII character set.

   A FEN string contains six fields separated by a space. The fields are:

   1) Piece placement (from white's perspective). Each rank is described, starting
      with rank 9 and ending with rank 0. Within each rank, the contents of each
      square are described from file A through file I. Following the Standard
      Algebraic Notation (SAN), each piece is identified by a single letter taken
      from the standard English names. White pieces are designated using upper-case
      letters ("RACPNBK") whilst Black uses lowercase ("racpnbk"). Blank squares are
      noted using digits 1 through 9 (the number of blank squares), and "/"
      separates ranks.

   2) Active color. "w" means white moves next, "b" means black.

   3) Halfmove clock. This is the number of halfmoves since the last pawn advance
      or capture. This is used to determine if a draw can be claimed under the
      fifty-move rule.

   4) Fullmove number. The number of the full move. It starts at 1, and is
      incremented after Black's move.
*/

    unsigned char token;
    size_t idx;
    Square sq = SQ_A9;
    std::istringstream ss(fenStr);

    // Free states before memset
    states = nullptr;
    std::memset(this, 0, sizeof(Position));
    // First StateInfo.previous -> nullptr
    states = StateListPtr(new std::deque<StateInfo>(1));
    st = &states->back();

    ss >> std::noskipws;

    // 1. Piece placement
    while ((ss >> token) && !isspace(token))
    {
        if (isdigit(token))
            sq += (token - '0') * EAST; // Advance the given number of files

        else if (token == '/')
            sq += 2 * SOUTH;

        else if ((idx = PieceToChar.find(token)) != string::npos) {
            put_piece(Piece(idx), sq);
            ++sq;
        }
    }

    // 2. Active color
    ss >> token;
    sideToMove = (token == 'w' ? WHITE : BLACK);
    ss >> token;

    while ((ss >> token) && !isspace(token));

    while ((ss >> token) && !isspace(token));

    // 3-4. Halfmove clock and fullmove number
    ss >> std::skipws >> token >> gamePly;

    // Convert from fullmove starting from 1 to gamePly starting from 0,
    // handle also common incorrect FEN with fullmove = 0.
    gamePly = std::max(2 * (gamePly - 1), 0) + (sideToMove == BLACK);

    return *this;
}


/// Position::fen() returns a FEN representation of the position.
string Position::fen() const {
    int emptyCnt;
    std::ostringstream ss;

    for (Rank r = RANK_9; r >= RANK_0; --r)
    {
        for (File f = FILE_A; f <= FILE_I; ++f)
        {
            for (emptyCnt = 0; f <= FILE_I && empty(make_square(f, r)); ++f)
                ++emptyCnt;

            if (emptyCnt)
                ss << emptyCnt;

            if (f <= FILE_I)
                ss << PieceToChar[piece_on(make_square(f, r))];
        }

        if (r > RANK_0)
            ss << '/';
    }

    ss << (sideToMove == WHITE ? " w " : " b ");

    ss << '-';

    ss << " - " << 0 << " " << 1 + (gamePly - (sideToMove == BLACK)) / 2;

    return ss.str();
}


void Position::do_move(Move m) {
    assert(is_ok(m));
    states->emplace_back();
    do_move(m, states->back());
}


/// Position::do_move() makes a move, and saves all information necessary
/// to a StateInfo object. The move is assumed to be legal. Pseudo-legal
/// moves should be filtered out before this function is called.
void Position::do_move(Move m, StateInfo& newSt) {
    assert(is_ok(m));
    assert(&newSt != st);

    newSt.previous = st;
    st = &newSt;

    // Used by NNUE
    st->accumulator.computed[WHITE] = false;
    st->accumulator.computed[BLACK] = false;
    auto& dp = st->dirtyPiece;
    dp.dirty_num = 1;

    Color us = sideToMove;
    Color them = ~us;
    Square from = from_sq(m);
    Square to = to_sq(m);
    Piece pc = piece_on(from);
    Piece captured = piece_on(to);

    assert(color_of(pc) == us);
    assert(captured == NO_PIECE || color_of(captured) == them);
    assert(type_of(captured) != KING);

    if (captured)
    {
        Square capsq = to;

        dp.dirty_num = 2;  // 1 piece moved, 1 piece captured
        dp.piece[1] = captured;
        dp.from[1] = capsq;
        dp.to[1] = SQ_NONE;

        // Update board and piece lists
        remove_piece(capsq);
    }

    // Move the piece.
    dp.piece[0] = pc;
    dp.from[0] = from;
    dp.to[0] = to;

    move_piece(from, to);

    // Set capture piece
    st->capturedPiece = captured;

    sideToMove = ~sideToMove;
}


/// Position::undo_move() unmakes a move. When it returns, the position should
/// be restored to exactly the same state as before the move was made.
void Position::undo_move(Move m) {
    assert(is_ok(m));

    sideToMove = ~sideToMove;

    Square from = from_sq(m);
    Square to = to_sq(m);

    assert(empty(from));
    assert(type_of(st->capturedPiece) != KING);

    move_piece(to, from); // Put the piece back at the source square

    if (st->capturedPiece)
    {
        Square capsq = to;

        put_piece(st->capturedPiece, capsq); // Restore the captured piece
    }

    // Finally point our state pointer back to the previous state
    st = st->previous;
    if (!states->empty())
        states->pop_back();
}


void Position::do_null_move() {
    states->emplace_back();
    do_null_move(states->back());
}


/// Position::do_null_move() is used to do a "null move": it flips
/// the side to move without executing any move on the board.
void Position::do_null_move(StateInfo& newSt) {
    assert(&newSt != st);

    std::memcpy(&newSt, st, offsetof(StateInfo, accumulator));

    newSt.previous = st;
    st = &newSt;

    st->dirtyPiece.dirty_num = 0;
    st->dirtyPiece.piece[0] = NO_PIECE; // Avoid checks in UpdateAccumulator()
    st->accumulator.computed[WHITE] = false;
    st->accumulator.computed[BLACK] = false;

    sideToMove = ~sideToMove;
}


/// Position::undo_null_move() must be used to undo a "null move"
void Position::undo_null_move() {
    st = st->previous;
    if (!states->empty())
        states->pop_back();
    sideToMove = ~sideToMove;
}

} // namespace Stockfish
