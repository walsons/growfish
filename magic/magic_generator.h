#ifndef MAGIC_GENERATOR_H
#define MAGIC_GENERATOR_H

#include <cstring>
#include <random>
#include <iostream>
#include <sstream>

#include "../src/magic_base.h"
#include "../src/thread_pool.h"

class MagicGenerator : public MagicBase
{
public:
    template <PieceType pt>
    void PrintMagicArray() const;
    void PrintLSBMagic() const;

private:
    Bitboard find_lsb_magic() const;

    template <PieceType pt>
    Bitboard find_magic(Square s) const;

    Bitboard random_bitboard() const;
    Bitboard random_bitboard_few_bits() const;

    U64 bitboard_high_64bit(Bitboard b) const { return (b >> 64); }
    U64 bitboard_low_64bit(Bitboard b) const { return b & (0ULL - 1); }

private:
    mutable std::default_random_engine e;
};

template <PieceType pt>
void MagicGenerator::PrintMagicArray() const
{
    std::string magicName;
    if constexpr (pt == PieceType::ROOK)
        magicName = "RookMagicNumber";
    else if (pt == PieceType::CANNON)
        magicName = "CannonMagicNumber";
    else if (pt == PieceType::KNIGHT)
        magicName = "KnightMagicNumber";
    else if (pt == PieceType::KNIGHT_TO)
        magicName = "KnightToMagicNumber";
    else if (pt == PieceType::BISHOP)
        magicName = "BishopMagicNumber";
    else
    {
        std::cout << "Unknown piece type" << std::endl;
        return;
    }

    ThreadPool magicPool(8);
    std::vector<std::future<std::string>> results;
    // According to experiment, SQ_G2 cost much more time than others 
    for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST) 
    {
        results.emplace_back(magicPool.Submit([this, s]() {
            std::ostringstream oss;
            oss << std::hex;
            Bitboard magic = find_magic<pt>(s);
            oss << "    (Bitboard(0x" << bitboard_high_64bit(magic) << "ULL) << 64) | Bitboard(0x" << bitboard_low_64bit(magic) << "ULL),\n";
            return oss.str();
        }));
    }

    std::cout << "constexpr Bitboard " << magicName << "[SQ_NUM] = \n{" << std::endl;
    for (auto &result : results)
        std::cout << result.get();
    std::cout << "};" << std::endl;
}

template <PieceType pt>
Bitboard MagicGenerator::find_magic(Square s) const
{
    Bitboard mask = get_mask<pt>(s);
    size_t ones = BB1Count(mask);
    size_t size = 1ULL << ones;
    std::vector<Bitboard> indexs(size);
    std::vector<Bitboard> attacks(size);
    for (size_t i = 0; i < size; ++i)
    {
        indexs[i] = index_to_bitboard(i, ones, mask);
        attacks[i] = attack_bb<pt>(s, indexs[i]);
    }
    std::vector<Bitboard> used(size);
    while (true)
    {
        Bitboard magic = random_bitboard_few_bits();
        std::fill(used.begin(), used.end(), 0);
        bool fail = false;
        for (size_t i = 0; !fail && i < size; ++i)
        {
            size_t j = transform(indexs[i], magic, (128ULL - ones));
            if (used[j] == 0)
                used[j] = attacks[i];
            else if (used[j] != attacks[i])
                fail = true;
        }
        if (!fail)
        {
            return magic;
        }
    }
    return 0;
}

#endif // MAGIC_GENERATOR_H