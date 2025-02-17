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
#elif defined(__GNUC__) || defined(__clang__)  // Assumed gcc or compatible compiler
    if (static_cast<unsigned long long>(b))
        return Square(__builtin_ctzll(b));
    return Square(__builtin_ctzll(b >> 64) + 64);
#else  // Also fast enough
    static constexpr Bitboard LSBMagicNumber = (Bitboard(0x152c5e643940449dULL) << 64) | Bitboard(0x5da6cf7ee2cbf147ULL);
    static constexpr Bitboard LSBMagicIndexArray[128] =
    {
        42, 43, 44, 0, 45, 0, 0, 30, 46, 50, 0, 14, 0, 0, 31, 0,
        27, 47, 51, 54, 37, 1, 8, 15, 0, 23, 0, 75, 32, 57, 82, 0,
        40, 28, 48, 0, 52, 6, 73, 55, 38, 4, 2, 62, 9, 0, 64, 16,
        0, 11, 24, 79, 70, 0, 76, 0, 0, 33, 58, 66, 18, 83, 0, 88,
        41, 0, 0, 29, 49, 13, 0, 0, 26, 53, 36, 7, 22, 74, 56, 81,
        39, 0, 5, 72, 3, 61, 0, 63, 10, 78, 69, 0, 0, 65, 17, 87,
        0, 0, 12, 0, 25, 35, 21, 80, 0, 71, 60, 0, 77, 68, 0, 86,
        0, 0, 34, 20, 0, 59, 67, 85, 0, 19, 0, 84, 0, 0, 89, 0,
    };
    static constexpr Bitboard shift = 128 - 7;
    return Square(LSBMagicIndexArray[((b & -b) * LSBMagicNumber) >> shift]);
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