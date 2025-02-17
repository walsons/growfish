#include "magic_generator.h"

void MagicGenerator::PrintLSBMagic() const
{
    Bitboard magic = find_lsb_magic();
    std::cout << std::hex;
    std::cout << "constexpr Bitboard LSBMagicNumber = "
                << "(Bitboard(0x" << bitboard_high_64bit(magic) << "ULL) << 64) | Bitboard(0x" << bitboard_low_64bit(magic) << "ULL)"
                << ";" << std::endl;
    std::cout << "constexpr Bitboard LSBMagicIndexArray[128] = \n{";
    std::vector<size_t> indices(128, 0);
    for (size_t i = 0; i < 90; ++i)
    {
        size_t index = (((Bitboard(1) << i) * magic) >> (128 - 7));
        indices[index] = i;
    }
    std::cout << std::dec;
    for (size_t i = 0; i < indices.size(); ++i)
    {
        if (i % 16 == 0)
            std::cout << "\n    ";
        std::cout << indices[i] << ", ";
    }
    std::cout << "\n};" << std::endl;
}

Bitboard MagicGenerator::find_lsb_magic() const
{
    while (true)
    {
        Bitboard used = 0;
        Bitboard magic = random_bitboard();
        bool fail = false;
        for (size_t i = 0; i < 90 && !fail; ++i)
        {
            // 7 bit can be used to index 0 ~ 127, but we only need 0 ~ 90
            size_t index = (((Bitboard(1) << i) * magic) >> (128 - 7));
            if (used & (Bitboard(1) << index))
                fail = true;
            else
                used |= (Bitboard(1) << index);
        }
        if (!fail)
            return magic;
    }
}

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