#ifndef BITBOARD_H
#define BITBOARD_H

#include "types.h"

/* Palace:
    000111000
    000111000
    000111000
    000000000
    000000000
    000000000
    000000000
    000111000
    000111000
    000111000
*/
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

inline constexpr Bitboard FileBB(File f) { return FileABB << f; }
inline constexpr Bitboard RankBB(Rank r) { return Rank0BB << (FILE_NB * r); }

constexpr Bitboard RedRegion = Rank0BB | Rank1BB | Rank2BB | Rank3BB | Rank4BB;
constexpr Bitboard BlackRegion = Rank5BB | Rank6BB | Rank7BB | Rank8BB | Rank9BB;
 
inline constexpr Bitboard SquareBB(Square s) { return Bitboard(1ULL) << s; }

// Return the least significant bit in a non-zero bitboard
inline Square LSB(Bitboard b) {
    assert(b);
#if defined(_MSC_VER)  // MSVC
    unsigned long idx;
    if (b._Word[0])
    {
        _BitScanForward64(&idx, b._Word[0]);
        return Square(idx);
    }
    else
    {
        _BitScanForward64(&idx, b._Word[1]);
        return Square(idx + 64);
    }
#else  // Assumed gcc or compatible compiler
    if (static_cast<unsigned long long>(b))
        return Square(__builtin_ctzll(b));
    return Square(__builtin_ctzll(b >> 64) + 64);
#endif
}

inline Square PopLSB(Bitboard& b)
{
    assert(b);
    Square s = LSB(b);
    b &= b - 1;
    return s;
}

inline size_t BB1Count(Bitboard b)
{
    size_t i = 0;
    for (i = 0; b; ++i, b &= b - 1) {}
    return i;
}

// Show bitboard in piece board format for test
void ShowBitboard(Bitboard b);

#endif