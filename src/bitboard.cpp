#include "bitboard.h"
#include <iostream>

#include "magic.h"

Bitboard LINE_BB[SQ_NUM][SQ_NUM];
Bitboard BETWEEN_BB[SQ_NUM][SQ_NUM]; 
void InitBB()
{
    for (Square s1 = SQ_A0; s1 <= SQ_NUM; s1 += SQ_NORTH)
    {
        for (Square s2 = SQ_A0; s2 <= SQ_NUM; s2 += SQ_NORTH)
        {
            LINE_BB[s1][s2] = (Attack<PieceType::ROOK>(s1) & Attack<PieceType::ROOK>(s2)) | SquareBB(s1) | SquareBB(s2);
            BETWEEN_BB[s1][s2] = (Attack<PieceType::ROOK>(SquareBB(s2), s1) & Attack<PieceType::ROOK>(SquareBB(s1), s2));
        }
    }
}
Bitboard LineBB(Square s1, Square s2)
{
    return LINE_BB[s1][s2];
}
Bitboard BetweenBB(Square s1, Square s2)
{
    return BETWEEN_BB[s1][s2];
}
bool MoreThan1Bit(Bitboard b)
{
    return b & (b - 1);
}

void ShowBitboard(Bitboard b)
{
    for (int r = Rank::RANK_9; r >= Rank::RANK_0; --r)
    {
        for (int f = File::FILE_A; f < File::FILE_NB; ++f)
        {
            std::cout << (((Bitboard(1) << (r * FILE_NB + f)) & b) ? "1" : "0");
        }
        std::cout << std::endl;
    }
    std::cout << "------------" << std::endl;
}

// unsigned countTwoByteOnes[1 << 16];
// union CountOnesHelper
// {
//     Bitboard b;
//     unsigned short s[8];
// };
// unsigned countOnes(Bitboard b)
// {
//     CountOnesHelper c = {b};
//     return countTwoByteOnes[c.s[0]] + countTwoByteOnes[c.s[1]]
//          + countTwoByteOnes[c.s[2]] + countTwoByteOnes[c.s[3]]
//          + countTwoByteOnes[c.s[4]] + countTwoByteOnes[c.s[5]]
//          + countTwoByteOnes[c.s[6]] + countTwoByteOnes[c.s[7]];
// }


// void BitboardInit()
// {
//     for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST) 
//     {
//         SquareBB[s] = Bitboard(1ULL) << s;
//     }
//     for (size_t i = 0; i < (1 << 16); ++i)
//     {
//         Bitboard num = i;
//         unsigned count = 0;
//         while (num)
//         {
//             PopLSB(num);
//             ++count;
//         }
//         countTwoByteOnes[i] = count;
//     }
//     for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST) 
//     {
//         Bitboard rookFileMask = ((~Rank0BB) & (~Rank9BB)) & FileBB(FileOf(s));
//         Bitboard rookRankMask = ((~FileABB) & (~FileIBB)) & RankBB(RankOf(s));
//         RookMagic[s].mask = ~SquareBB[s] & (rookFileMask | rookRankMask);
//         RookMagic[s].shift = 128 - countOnes(RookMagic[s].mask);
//         RookMagic[s].magic_number = RookMagicNumber[s];
//         for (size_t i = 0; i < (1ULL << 15); ++i)
//         {

//         }
//     }

// }