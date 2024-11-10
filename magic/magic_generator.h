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
            magicName = "RookMagic";
        else if (pt == PieceType::CANNON)
            magicName = "CannonMagic";
        else if (pt == PieceType::KNIGHT)
            magicName = "KnightMagic";
        else if (pt == PieceType::BISHOP)
            magicName = "BishopMagic";
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
        // Only works for rook and cannon, other piece need a specialization
        static_assert(pt == PieceType::ROOK || pt == PieceType::CANNON, "Unsupported piece type");

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

    size_t count_1s(Bitboard b) const;
    Bitboard index_to_bitboard(size_t index, size_t bits, Bitboard mask) const;
    Bitboard random_bitboard();
    Bitboard random_bitboard_few_bits();

    Bitboard transform(Bitboard b, Bitboard magic, int bits) const { return (b * magic) >> (128 - bits); }
    uint_64 bitboard_high_64bit(Bitboard b) const { return (b >> 64); }
    uint_64 bitboard_low_64bit(Bitboard b) const { return b & (0ULL - 1); }

private:
    std::default_random_engine e;
};

template <>
inline Bitboard MagicGenerator::get_mask<PieceType::KNIGHT>(Square s) const
{
    Bitboard mask = 0;
    int rk = RankOf(s), fl = FileOf(s);
    Bitboard sBB = Bitboard(1) << s;
    Bitboard border = FileABB | FileIBB | RANK_0 | RANK_9;
    if (sBB & border)
    {
        int north = rk + 1, south = rk - 1, east = fl + 1, west = fl - 1;
        if (north < RANK_NB)
            mask |= Bitboard(1) << (north * FILE_NB + fl);
        if (south >= RANK_0)
            mask |= Bitboard(1) << (south * FILE_NB + fl);
        if (east < FILE_NB)
            mask |= Bitboard(1) << (rk * FILE_NB + east);
        if (west >= FILE_A)
            mask |= Bitboard(1) << (rk * FILE_NB + west);
    }
    else
    {
        int north = rk + 1, south = rk - 1, east = fl + 1, west = fl - 1;
        if (north < RANK_NB - 1)
            mask |= Bitboard(1) << (north * FILE_NB + fl);
        if (south > RANK_0)
            mask |= Bitboard(1) << (south * FILE_NB + fl);
        if (east < FILE_NB - 1)
            mask |= Bitboard(1) << (rk * FILE_NB + east);
        if (west > FILE_A)
            mask |= Bitboard(1) << (rk * FILE_NB + west);
    }
    return mask;
}

template <>
inline Bitboard MagicGenerator::get_mask<PieceType::BISHOP>(Square s) const
{
    Bitboard mask = 0;
    constexpr int blocks[4] = {SQ_NORTH + SQ_EAST, SQ_EAST + SQ_SOUTH, SQ_SOUTH + SQ_WEST, SQ_WEST + SQ_NORTH};
    for (auto block : blocks)
    {
        if (Distance(s, static_cast<Square>(s + block)) <= 1 && (s + block) >= SQ_A0 && (s + block) < SQ_NUM)
        {
            if (s < SQ_A5 && (s + block) < SQ_A5)
                mask |= (Bitboard(1) << (s + block));
            else if (s >= SQ_A5 && (s + block) >= SQ_A5)
                mask |= (Bitboard(1) << (s + block));
        }
    }
    return mask;
}

template <>
inline Bitboard MagicGenerator::attack<PieceType::ROOK>(Square s, Bitboard occupies) const
{
    Bitboard result = 0;
    int rk = RankOf(s), fl = FileOf(s);
    for (int r = rk + 1; r < RANK_NB; r++) 
    {
        result |= Bitboard(1) << (r * FILE_NB + fl);
        if (occupies & (Bitboard(1) << (r * FILE_NB + fl)))
            break;
    }
    for (int r = rk - 1; r >= RANK_0; r--) 
    {
        result |= Bitboard(1) << (r * FILE_NB + fl);
        if (occupies & (Bitboard(1) << (r * FILE_NB + fl)))
            break;
    }
    for (int f = fl + 1; f < FILE_NB; f++) 
    {
        result |= Bitboard(1) << (rk * FILE_NB + f);
        if (occupies & (Bitboard(1) << (rk * FILE_NB + f)))
            break;
    }
    for (int f = fl - 1; f >= FILE_A; f--) 
    {
        result |= Bitboard(1) << (rk * FILE_NB + f);
        if (occupies & (Bitboard(1) << (rk * FILE_NB + f)))
            break;
    }
    return result;
}

template <>
inline Bitboard MagicGenerator::attack<PieceType::CANNON>(Square s, Bitboard occupies) const
{
    Bitboard result = 0;
    int rk = RankOf(s), fl = FileOf(s);
    bool bridge = false;
    for (int r = rk + 1; r < RANK_NB; r++) 
    {
        if (bridge)
            result |= Bitboard(1) << (r * FILE_NB + fl);
        if (occupies & (Bitboard(1) << (r * FILE_NB + fl)))
        {
            if (!bridge)
                bridge = true;
            else
                break;
        }
    }
    bridge = false;
    for (int r = rk - 1; r >= RANK_0; r--) 
    {
        if (bridge)
            result |= Bitboard(1) << (r * FILE_NB + fl);
        if (occupies & (Bitboard(1) << (r * FILE_NB + fl)))
        {
            if (!bridge)
                bridge = true;
            else
                break;
        }
    }
    bridge = false;
    for (int f = fl + 1; f < FILE_NB; f++) 
    {
        if (bridge)
            result |= Bitboard(1) << (rk * FILE_NB + f);
        if (occupies & (Bitboard(1) << (rk * FILE_NB + f)))
        {
            if (!bridge)
                bridge = true;
            else
                break;
        }
    }
    bridge = false;
    for (int f = fl - 1; f >= FILE_A; f--) 
    {
        if (bridge)
            result |= Bitboard(1) << (rk * FILE_NB + f);
        if (occupies & (Bitboard(1) << (rk * FILE_NB + f)))
        {
            if (!bridge)
                bridge = true;
            else
                break;
        }
    }
    return result;
}

template <>
inline Bitboard MagicGenerator::attack<PieceType::KNIGHT>(Square s, Bitboard occupies) const
{
    Bitboard result = 0;
    constexpr int forwards[8] = {2 * SQ_NORTH + SQ_WEST, 2 * SQ_NORTH + SQ_EAST, 
                                 SQ_NORTH + 2 * SQ_EAST, SQ_SOUTH + 2 * SQ_EAST, 
                                 2 * SQ_SOUTH + SQ_EAST, 2 * SQ_SOUTH + SQ_WEST, 
                                 SQ_SOUTH + 2 * SQ_WEST, SQ_NORTH + 2 * SQ_WEST}; 
    constexpr int blocks[4] = {SQ_NORTH, SQ_EAST, SQ_SOUTH, SQ_WEST};
    for (int i = 0; i < 8; ++i)
    {
        if (Distance(s, static_cast<Square>(s + forwards[i])) <= 2 && (s + forwards[i]) >= SQ_A0 && (s + forwards[i]) < SQ_NUM)
        {
            if (!(occupies & (Bitboard(1) << (s + blocks[i / 2]))))
                result |= (Bitboard(1) << (s + forwards[i]));
        }
    }
    return result;
}

template <>
inline Bitboard MagicGenerator::attack<PieceType::BISHOP>(Square s, Bitboard occupies) const
{
    Bitboard result = 0;
    constexpr int forwards[4] = {2 * SQ_NORTH + 2 * SQ_EAST,
                                 2 * SQ_EAST + 2 * SQ_SOUTH,
                                 2 * SQ_SOUTH + 2 * SQ_WEST,
                                 2 * SQ_WEST + 2 * SQ_NORTH};
    constexpr int blocks[4] = {SQ_NORTH + SQ_EAST, SQ_EAST + SQ_SOUTH, SQ_SOUTH + SQ_WEST, SQ_WEST + SQ_NORTH};
    for (int i = 0; i < 4; ++i)
    {
        if (Distance(s, static_cast<Square>(s + forwards[i])) <= 2 && (s + forwards[i]) >= SQ_A0 && (s + forwards[i]) < SQ_NUM)
        {
            if (s < SQ_A5 && (s + forwards[i]) < SQ_A5)
            {
                if (!(occupies & (Bitboard(1) << (s + blocks[i]))))
                    result |= (Bitboard(1) << (s + forwards[i]));
            }
            else if (s >= SQ_A5 && (s + forwards[i]) >= SQ_A5)
            {
                if (!(occupies & (Bitboard(1) << (s + blocks[i]))))
                    result |= (Bitboard(1) << (s + forwards[i]));
            }
        }
    }
    return result;
}

#endif