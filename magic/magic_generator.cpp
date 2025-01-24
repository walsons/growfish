#include "magic_generator.h"

Bitboard MagicGenerator::random_bitboard() const
{
    // e() range from 0x1 to 0x7ffffffe
    return e() | (Bitboard(e()) << 31) | (Bitboard(e()) << 62) 
               | (Bitboard(e()) << 93) | (Bitboard(e()) << 124);
}

Bitboard MagicGenerator::random_bitboard_few_bits() const
{
    return random_bitboard() & random_bitboard() & random_bitboard();
}