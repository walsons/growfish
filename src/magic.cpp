#include "magic.h"


size_t MagicInitializer::count_1s(Bitboard b) const
{
    size_t i = 0;
    for (i = 0; b; ++i, b &= b - 1) {}
    return i;
}
Bitboard MagicInitializer::index_to_bitboard(size_t index, size_t bits, Bitboard mask) const
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

Bitboard AttackInitializer::PawnAttackBB[NUM(Color::COLOR_NUM)][SQ_NUM] = {};
Bitboard AttackInitializer::PawnToAttackBB[NUM(Color::COLOR_NUM)][SQ_NUM] = {};
Bitboard AttackInitializer::AdvisorAttackBB[SQ_NUM] = {};
Bitboard AttackInitializer::KingAttackBB[SQ_NUM] = {};
