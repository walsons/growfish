#include "bitboard_cache.h"

BitboardCache::BitboardCache()
{
    AttackInitializer::InitAttack();

    for (Square s1 = SQ_A0; s1 < SQ_NUM; s1 += SQ_EAST)
    {
        for (Square s2 = SQ_A0; s2 < SQ_NUM; s2 += SQ_EAST)
        {
            if (RankOf(s1) == RankOf(s2) || FileOf(s1) == FileOf(s2))  // Rook and Cannon
                between_bb_[s1][s2] = (AttackBB<PieceType::ROOK>(s1, SquareBB(s2)) & AttackBB<PieceType::ROOK>(s2, SquareBB(s1)));
            else  // KNIGHT_TO, others can be neglect
            {
                auto r1 = RankOf(s1), r2 = RankOf(s2);
                auto c1 = FileOf(s1), c2 = FileOf(s2);
                Square barrier = s1;
                if (r2 > r1)
                    barrier += SQ_NORTH;
                else
                    barrier += SQ_SOUTH;
                if (c2 > c1)
                    barrier += SQ_EAST;
                else
                    barrier += SQ_WEST;
                between_bb_[s1][s2] = SquareBB(barrier);
            }
        }
    }
}

BitboardCache BITBOARD_CACHE;

Bitboard BetweenBB(Square s1, Square s2)
{
    return BITBOARD_CACHE.between_bb_[s1][s2];
}