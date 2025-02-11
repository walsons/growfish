#include "bitboard.h"
#include <iostream>

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
