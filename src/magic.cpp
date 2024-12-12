#include "magic.h"

// Magic RookMagic[SQ_NUM];
// Magic CannonMagic[SQ_NUM];
// Magic KnightMagic[SQ_NUM];
// Magic BishopMagic[SQ_NUM];

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

template <>
Bitboard AttackInitializer::PawnAttackBB<Color::RED>[SQ_NUM] = {};
template <>
Bitboard AttackInitializer::PawnAttackBB<Color::BLACK>[SQ_NUM] = {};
template <>
Bitboard AttackInitializer::PawnToAttackBB<Color::RED>[SQ_NUM] = {};
template <>
Bitboard AttackInitializer::PawnToAttackBB<Color::BLACK>[SQ_NUM] = {};
Bitboard AttackInitializer::AdvisorAttackBB[SQ_NUM];
Bitboard AttackInitializer::KingAttackBB[SQ_NUM];
