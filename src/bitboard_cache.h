#ifndef BITBOARD_CACHE_H
#define BITBOARD_CACHE_H

#include "magic.h"

class BitboardCache
{
public:
    BitboardCache();

    Bitboard between_bb_[SQ_NUM][SQ_NUM]; 
};

Bitboard BetweenBB(Square s1, Square s2);

#endif