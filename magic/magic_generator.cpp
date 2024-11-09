#include "magic_generator.h"

size_t MagicGenerator::count_1s(Bitboard b) const
{
    size_t i = 0;
    for (i = 0; b; ++i, b &= b - 1) {}
    return i;
}
Bitboard MagicGenerator::index_to_bitboard(size_t index, size_t bits, Bitboard mask) const
{
    Bitboard result = 0;
    for (size_t i = 0; i < bits; ++i)
    {
        size_t j = PopLSB(mask);
        if (index & (1 << i))
            result |= Bitboard(1) << j;
    }
    return result;
}

Bitboard MagicGenerator::random_bitboard()
{
    // e() range from 0x1 to 0x7ffffffe
    return e() | (Bitboard(e()) << 31) | (Bitboard(e()) << 62) 
                | (Bitboard(e()) << 93) | (Bitboard(e()) << 124);
}

Bitboard MagicGenerator::random_bitboard_few_bits()
{
    return random_bitboard() & random_bitboard() & random_bitboard();
}