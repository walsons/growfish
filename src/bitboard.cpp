#include "bitboard.h"

Bitboard SquareBB[SQ_NUM];

void Bitboard_init()
{
    for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST) 
    {
        SquareBB[s] = Bitboard(1ULL) << s;
    }
}