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
#include <bitset>

#include "bitboard.h"
#include "misc.h"
#include <set>
#include <iostream>

namespace Stockfish {

uint8_t PopCnt16[1 << 16];
uint8_t SquareDistance[SQUARE_NB][SQUARE_NB];

Bitboard SquareBB[SQUARE_NB];
Bitboard LineBB[SQUARE_NB][SQUARE_NB];
Bitboard BetweenBB[SQUARE_NB][SQUARE_NB];
Bitboard PseudoAttacks[PIECE_TYPE_NB][SQUARE_NB];
Bitboard FullAttacks[PIECE_TYPE_NB][SQUARE_NB];
Bitboard PawnAttacks[COLOR_NB][SQUARE_NB];
Bitboard PawnAttacksTo[COLOR_NB][SQUARE_NB];

Magic RookMagics[SQUARE_NB];
Magic CannonMagics[SQUARE_NB];
Magic BishopMagics[SQUARE_NB];
Magic KnightMagics[SQUARE_NB];
Magic KnightToMagics[SQUARE_NB];

namespace {

  const std::set<Direction> KnightDirections { 2 * SOUTH + WEST, 2 * SOUTH + EAST, SOUTH + 2 * WEST, SOUTH + 2 * EAST,
                                               NORTH + 2 * WEST, NORTH + 2 * EAST, 2 * NORTH + WEST, 2 * NORTH + EAST };
  const std::set<Direction> BishopDirections { 2 * NORTH_EAST, 2 * SOUTH_EAST, 2 * SOUTH_WEST, 2 * NORTH_WEST };


  template <PieceType pt>
  void init_magics(Bitboard table[], Magic magics[], const Bitboard magicsInit[]);

  template <PieceType pt>
  Bitboard lame_leaper_path(Direction d, Square s);

}

/// safe_destination() returns the bitboard of target square for the given step
/// from the given square. If the step is off the board, returns empty bitboard.

inline Bitboard safe_destination(Square s, int step) {
    Square to = Square(s + step);
    return is_ok(to) && distance(s, to) <= 2 ? square_bb(to) : Bitboard(0);
}


/// Bitboards::pretty() returns an ASCII representation of a bitboard suitable
/// to be printed to standard output. Useful for debugging.

std::string Bitboards::pretty(Bitboard b) {

  std::string s = "+---+---+---+---+---+---+---+---+---+\n";

  for (Rank r = RANK_9; r >= RANK_0; --r)
  {
      for (File f = FILE_A; f <= FILE_I; ++f)
          s += b & make_square(f, r) ? "| X " : "|   ";

      s += "| " + std::to_string(r) + "\n+---+---+---+---+---+---+---+---+---+\n";
  }
  s += "  a   b   c   d   e   f   g   h   i\n";

  return s;
}


/// Bitboards::init() initializes various bitboard tables. It is called at
/// startup and relies on global objects to be already zero-initialized.

void Bitboards::init() {

  for (unsigned i = 0; i < (1 << 16); ++i)
      PopCnt16[i] = uint8_t(std::bitset<16>(i).count());

  for (Square s = SQ_A0; s <= SQ_I9; ++s)
      SquareBB[s] = (Bitboard(1ULL) << s);

  for (Square s1 = SQ_A0; s1 <= SQ_I9; ++s1)
      for (Square s2 = SQ_A0; s2 <= SQ_I9; ++s2)
          SquareDistance[s1][s2] = std::max(distance<File>(s1, s2), distance<Rank>(s1, s2));
}

namespace {

  template <PieceType pt>
  Bitboard sliding_attack(Square sq, Bitboard occupied) {
    assert(pt == ROOK || pt == CANNON);
    Bitboard attack = 0;

    for (auto const& d : { NORTH, SOUTH, EAST, WEST } )
    {
      bool hurdle = false;
      for (Square s = sq + d; is_ok(s) && distance(s - d, s) == 1; s += d)
      {
        if (pt == ROOK || hurdle)
          attack |= s;

        if (occupied & s)
        {
          if (pt == CANNON && !hurdle)
            hurdle = true;
          else
            break;
        }
      }
    }

    return attack;
  }

  template <PieceType pt>
  Bitboard lame_leaper_path(Direction d, Square s) {
    Bitboard b = 0;
    Square to = s + d;
    if (!is_ok(to) || distance(s, to) >= 4)
        return b;

    // If piece type is by knight attacks, swap the source and destination square
    if (pt == KNIGHT_TO) {
      std::swap(s, to);
      d = -d;
    }

    Direction dr = d > 0 ? NORTH : SOUTH;
    Direction df = (std::abs(d % NORTH) < NORTH / 2 ? d % NORTH : -(d % NORTH)) < 0 ? WEST : EAST;

    int diff = std::abs(file_of(to) - file_of(s)) - std::abs(rank_of(to) - rank_of(s));
    if (diff > 0)
      s += df;
    else if (diff < 0)
      s += dr;
    else
      s += df + dr;

    b |= s;
    return b;
  }

  template <PieceType pt>
  Bitboard lame_leaper_path(Square s) {
    Bitboard b = 0;
    for (const auto& d : pt == BISHOP ? BishopDirections : KnightDirections)
      b |= lame_leaper_path<pt>(d, s);
    if (pt == BISHOP)
      b &= HalfBB[rank_of(s) > RANK_4];
    return b;
  }

  template <PieceType pt>
  Bitboard lame_leaper_attack(Square s, Bitboard occupied) {
    Bitboard b = 0;
    for (const auto& d : pt == BISHOP  ? BishopDirections : KnightDirections)
    {
      Square to = s + d;
      if (is_ok(to) && distance(s, to) < 4 && !(lame_leaper_path<pt>(d, s) & occupied))
        b |= to;
    }
    if (pt == BISHOP)
      b &= HalfBB[rank_of(s) > RANK_4];
    return b;
  }

}

} // namespace Stockfish
