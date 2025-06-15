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

#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include <string>

#include "types.h"

namespace Stockfish {

namespace Bitboards {

void init();
std::string pretty(Bitboard b);

} // namespace Stockfish::Bitboards

constexpr Bitboard Palace = Bitboard(0x70381CULL) << 64 | Bitboard(0xE07038ULL);

constexpr Bitboard FileABB = Bitboard(0x20100ULL) << 64 | Bitboard(0x8040201008040201ULL);
constexpr Bitboard FileBBB = FileABB << 1;
constexpr Bitboard FileCBB = FileABB << 2;
constexpr Bitboard FileDBB = FileABB << 3;
constexpr Bitboard FileEBB = FileABB << 4;
constexpr Bitboard FileFBB = FileABB << 5;
constexpr Bitboard FileGBB = FileABB << 6;
constexpr Bitboard FileHBB = FileABB << 7;
constexpr Bitboard FileIBB = FileABB << 8;

constexpr Bitboard Rank0BB = 0x1FF;
constexpr Bitboard Rank1BB = Rank0BB << (FILE_NB * 1);
constexpr Bitboard Rank2BB = Rank0BB << (FILE_NB * 2);
constexpr Bitboard Rank3BB = Rank0BB << (FILE_NB * 3);
constexpr Bitboard Rank4BB = Rank0BB << (FILE_NB * 4);
constexpr Bitboard Rank5BB = Rank0BB << (FILE_NB * 5);
constexpr Bitboard Rank6BB = Rank0BB << (FILE_NB * 6);
constexpr Bitboard Rank7BB = Rank0BB << (FILE_NB * 7);
constexpr Bitboard Rank8BB = Rank0BB << (FILE_NB * 8);
constexpr Bitboard Rank9BB = Rank0BB << (FILE_NB * 9);


constexpr Bitboard PawnFileBB = FileABB | FileCBB | FileEBB | FileGBB | FileIBB;
constexpr Bitboard HalfBB[2] = { Rank0BB | Rank1BB | Rank2BB | Rank3BB | Rank4BB,
                                 Rank5BB | Rank6BB | Rank7BB | Rank8BB | Rank9BB };
constexpr Bitboard PawnBB[2] = { HalfBB[BLACK] | ((Rank3BB | Rank4BB) & PawnFileBB),
                                 HalfBB[WHITE] | ((Rank6BB | Rank5BB) & PawnFileBB) };

extern uint8_t PopCnt16[1 << 16];
extern uint8_t SquareDistance[SQUARE_NB][SQUARE_NB];

extern Bitboard SquareBB[SQUARE_NB];

int popcount(Bitboard b); // required for 128 bit pext

/// Magic holds all magic bitboards relevant data for a single square
struct Magic {
  Bitboard  mask;
  Bitboard  magic;
  Bitboard* attacks;
  unsigned  shift;

  // Compute the attack's index using the 'magic bitboards' approach
  unsigned index(Bitboard occupied) const {

    if (HasPext)
        return unsigned(pext(occupied, mask, shift));

    return unsigned(((occupied & mask) * magic) >> shift);
  }
};

extern Magic RookMagics[SQUARE_NB];
extern Magic CannonMagics[SQUARE_NB];
extern Magic BishopMagics[SQUARE_NB];
extern Magic KnightMagics[SQUARE_NB];
extern Magic KnightToMagics[SQUARE_NB];

inline Bitboard square_bb(Square s) {
  assert(is_ok(s));
  return SquareBB[s];
}


/// Overloads of bitwise operators between a Bitboard and a Square for testing
/// whether a given bit is set in a bitboard, and for setting and clearing bits.

inline Bitboard  operator&( Bitboard  b, Square s) { return b &  square_bb(s); }
inline Bitboard  operator|( Bitboard  b, Square s) { return b |  square_bb(s); }
inline Bitboard  operator^( Bitboard  b, Square s) { return b ^  square_bb(s); }
inline Bitboard& operator|=(Bitboard& b, Square s) { return b |= square_bb(s); }
inline Bitboard& operator^=(Bitboard& b, Square s) { return b ^= square_bb(s); }

inline Bitboard  operator&(Square s, Bitboard b) { return b & s; }
inline Bitboard  operator|(Square s, Bitboard b) { return b | s; }
inline Bitboard  operator^(Square s, Bitboard b) { return b ^ s; }

inline Bitboard  operator|(Square s1, Square s2) { return square_bb(s1) | s2; }

constexpr bool more_than_one(Bitboard b) {
  return b & (b - 1);
}

inline Bitboard undo_move_board(Bitboard b, Move m) {
    return (from_sq(m) != SQ_NONE && (b & to_sq(m))) ? (b ^ to_sq(m)) | from_sq(m) : b;
}


/// rank_bb() and file_bb() return a bitboard representing all the squares on
/// the given file or rank.

constexpr Bitboard rank_bb(Rank r) {
  return Rank0BB << (FILE_NB * r);
}

constexpr Bitboard rank_bb(Square s) {
  return rank_bb(rank_of(s));
}

constexpr Bitboard file_bb(File f) {
  return FileABB << f;
}

constexpr Bitboard file_bb(Square s) {
  return file_bb(file_of(s));
}


/// shift() moves a bitboard one or two steps as specified by the direction D

template<Direction D>
constexpr Bitboard shift(Bitboard b) {
  return  D == NORTH      ? (b & ~Rank9BB           ) << NORTH       : D == SOUTH      ?  b             >> NORTH
        : D == NORTH+NORTH? (b & ~Rank9BB & ~Rank8BB) << NORTH+NORTH : D == SOUTH+SOUTH?  b             >> NORTH+NORTH
        : D == EAST       ? (b & ~FileIBB           ) << EAST        : D == WEST       ? (b & ~FileABB) >> EAST
        : D == NORTH_EAST ? (b & ~FileIBB           ) << NORTH_EAST  : D == NORTH_WEST ? (b & ~FileABB) << NORTH_WEST
        : D == SOUTH_EAST ? (b & ~FileIBB           ) >> NORTH_WEST  : D == SOUTH_WEST ? (b & ~FileABB) >> NORTH_EAST
        : Bitboard(0);
}


/// distance() functions return the distance between x and y, defined as the
/// number of steps for a king in x to reach y.

template<typename T1 = Square> inline int distance(Square x, Square y);
template<> inline int distance<File>(Square x, Square y) { return std::abs(file_of(x) - file_of(y)); }
template<> inline int distance<Rank>(Square x, Square y) { return std::abs(rank_of(x) - rank_of(y)); }
template<> inline int distance<Square>(Square x, Square y) { return SquareDistance[x][y]; }

inline int edge_distance(File f) { return std::min(f, File(FILE_H - f)); }
inline int edge_distance(Rank r) { return std::min(r, Rank(RANK_8 - r)); }


/// popcount() counts the number of non-zero bits in a bitboard

inline int popcount(Bitboard b) {

#ifndef USE_POPCNT

  union { Bitboard bb; uint16_t u[8]; } v = { b };
  return  PopCnt16[v.u[0]] + PopCnt16[v.u[1]] + PopCnt16[v.u[2]] + PopCnt16[v.u[3]]
         + PopCnt16[v.u[4]] + PopCnt16[v.u[5]] + PopCnt16[v.u[6]] + PopCnt16[v.u[7]];

#elif defined(_MSC_VER) || defined(__INTEL_COMPILER)

    return (int)_mm_popcnt_u64(b >> 64) + (int)_mm_popcnt_u64(b);

#else // Assumed gcc or compatible compiler

  return __builtin_popcountll(b >> 64) + __builtin_popcountll(b);

#endif
}


/// lsb() return the least significant bit in a non-zero bitboard

inline Square lsb(Bitboard b) {
  assert(b);

#if defined(_MSC_VER) // MSVC

  unsigned long idx;
  if (uint64_t(b))
  {
      _BitScanForward64(&idx, b);
      return Square(idx);
  }
  else
  {
      _BitScanForward64(&idx, b >> 64);
      return Square(idx + 64);
  }

#else // Assumed gcc or compatible compiler

    if (uint64_t(b))
        return Square(__builtin_ctzll(b));
    return Square(__builtin_ctzll(b >> 64) + 64);

#endif
}

/// least_significant_square_bb() returns the bitboard of the least significant
/// square of a non-zero bitboard. It is equivalent to square_bb(lsb(bb)).

inline Bitboard least_significant_square_bb(Bitboard b) {
  assert(b);
  return b & -b;
}

/// pop_lsb() finds and clears the least significant bit in a non-zero bitboard

inline Square pop_lsb(Bitboard& b) {
  assert(b);
  const Square s = lsb(b);
  b &= b - 1;
  return s;
}

} // namespace Stockfish

#endif // #ifndef BITBOARD_H_INCLUDED
