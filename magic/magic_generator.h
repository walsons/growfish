#ifndef MAGIC_GENERATOR_H
#define MAGIC_GENERATOR_H

#include <cstring>
#include <random>
#include <iostream>
#include "../src/bitboard.h"

class MagicGenerator
{
public:
    using uint_64 = unsigned long long;
    template <PieceType pt>
    void PrintMagicArray()
    {
        std::string magicName;
        if constexpr (pt == PieceType::ROOK)
        {
            magicName = "RookMagic";
        }
        // TODO: else if (pt == PieceType::CANNON )
        else
        {
            std::cout << "Unknown piece type" << std::endl;
            return;
        }
        std::cout << "constexpr Bitboard " << magicName << "[SQ_NUM] = \n{" << std::endl;
        std::cout << std::hex;
        for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST) 
        {
            Bitboard magic = find_magic<pt>(s);
            std::cout << "    (Bitboard(0x" << bitboard_high_64bit(magic) << "ULL) << 64) | Bitboard(0x" << bitboard_low_64bit(magic) << "ULL),\n";
        }
        std::cout << "};" << std::endl;
    }

private:
    template <PieceType pt>
    Bitboard get_mask(Square s) const
    {
        Bitboard mask = 0;
        int rk = RankOf(s), fl = FileOf(s);
        for (int r = rk + 1; r < RANK_NB - 1; r++) {
            mask |= Bitboard(1) << (r * FILE_NB + fl);
        }
        for (int r = rk - 1; r > RANK_0; r--) {
            mask |= Bitboard(1) << (r * FILE_NB + fl);
        }
        for (int f = fl + 1; f < FILE_NB - 1; f++) {
            mask |= Bitboard(1) << (rk * FILE_NB + f);
        }
        for (int f = fl - 1; f > FILE_A; f--) {
            mask |= Bitboard(1) << (rk * FILE_NB + f);
        }
        return mask;
    }
    template <PieceType pt>
    Bitboard attack(Square s, Bitboard occupies) const;
    template <PieceType pt>
    Bitboard find_magic(Square s)
    {
        Bitboard mask = get_mask<pt>(s);
        size_t ones = count_1s(mask);
        Bitboard* indexs = new Bitboard[(1 << ones)];
        Bitboard* attacks = new Bitboard[(1 << ones)];
        for (size_t i = 0; i < (1Ull << ones); ++i)
        {
            indexs[i] = index_to_bitboard(i, ones, mask);
            attacks[i] = attack<pt>(s, indexs[i]);
        }
        Bitboard* used = new Bitboard[(1 << ones)];
        constexpr size_t searchTimes = 100000000;
        for (size_t k = 0; k < searchTimes; ++k)
        {
            Bitboard magic = random_bitboard_few_bits();
            std::memset(used, 0, sizeof(used[0]) * (1 << ones));
            // for (int i = 0; i < (1 << ones); ++i)
            //     used[i] = 0; 
            bool fail = false;
            for (int i = 0; !fail && i < (1 << ones); ++i)
            {
                int j = transform(indexs[i], magic, ones);
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
        std::cout << "Failed! search " << searchTimes << " times" << std::endl;
        return 0;
    }

    size_t count_1s(Bitboard b) const
    {
        size_t i = 0;
        for (i = 0; b; ++i, b &= b - 1) {}
        return i;
    }
    Bitboard index_to_bitboard(size_t index, size_t bits, Bitboard mask) const
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
    Bitboard random_bitboard()
    {
        // e() range from 0x1 to 0x7ffffffe
        return e() | (Bitboard(e()) << 31) | (Bitboard(e()) << 62) 
                   | (Bitboard(e()) << 93) | (Bitboard(e()) << 124);
    }
    Bitboard random_bitboard_few_bits()
    {
        return random_bitboard() & random_bitboard() & random_bitboard();
    }
    Bitboard transform(Bitboard b, Bitboard magic, int bits)
    {
        return (b * magic) >> (128 - bits);
    }
    uint_64 bitboard_high_64bit(Bitboard b) const { return (b >> 64); }
    uint_64 bitboard_low_64bit(Bitboard b) const { return b & (0ULL - 1); }

private:
    std::default_random_engine e;
};

template <>
inline Bitboard MagicGenerator::attack<PieceType::ROOK>(Square s, Bitboard occupies) const
{
    Bitboard result = 0;
    int rk = RankOf(s), fl = FileOf(s);
    for (int r = rk + 1; r <= RANK_NB - 1; r++) {
        result |= Bitboard(1) << (r * FILE_NB + fl);
        if (occupies & (Bitboard(1) << (r * FILE_NB + fl)))
            break;
    }
    for (int r = rk - 1; r >= RANK_0 + 1; r--) {
        result |= Bitboard(1) << (r * FILE_NB + fl);
        if (occupies & (Bitboard(1) << (r * FILE_NB + fl)))
            break;
    }
    for (int f = fl + 1; f <= FILE_NB - 1; f++) {
        result |= Bitboard(1) << (rk * FILE_NB + f);
        if (occupies & (Bitboard(1) << (rk * FILE_NB + f)))
            break;
    }
    for (int f = fl - 1; f >= FILE_A + 1; f--) {
        result |= Bitboard(1) << (rk * FILE_NB + f);
        if (occupies & (Bitboard(1) << (rk * FILE_NB + f)))
            break;
    }
    return result;
}

#endif