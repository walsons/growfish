/*
 * Base class for magic generator(magic) and initializer(src)
 */

#ifndef MAGIC_BASE_H
#define MAGIC_BASE_H

#include "bitboard.h"


class MagicBase
{
protected:
    template <PieceType pt>
    Bitboard get_mask(Square s) const;

    template <PieceType pt>
    Bitboard attack_bb(Square s, Bitboard occupies) const;

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

    size_t transform(Bitboard occupancy, Bitboard magic, unsigned shift) const { return (occupancy * magic) >> shift; }
};

template <PieceType pt>
Bitboard MagicBase::get_mask(Square s) const
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

template <>
inline Bitboard MagicBase::get_mask<PieceType::KNIGHT>(Square s) const
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
inline Bitboard MagicBase::get_mask<PieceType::KNIGHT_TO>(Square s) const
{
    Bitboard mask = 0;
    int rk = RankOf(s), fl = FileOf(s);
    if (rk + 1 < RANK_NB && fl + 1 < FILE_NB)
        mask |= Bitboard(1) << ((rk + 1) * FILE_NB + fl + 1);
    if (rk + 1 < RANK_NB && fl - 1 >= FILE_A)
        mask |= Bitboard(1) << ((rk + 1) * FILE_NB + fl - 1);
    if (rk - 1 >= RANK_0 && fl + 1 < FILE_NB)
        mask |= Bitboard(1) << ((rk - 1) * FILE_NB + fl + 1);
    if (rk - 1 >= RANK_0 && fl - 1 >= FILE_A)
        mask |= Bitboard(1) << ((rk - 1) * FILE_NB + fl - 1);
    return mask;
}

template <>
inline Bitboard MagicBase::get_mask<PieceType::BISHOP>(Square s) const
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
inline Bitboard MagicBase::attack_bb<PieceType::ROOK>(Square s, Bitboard occupies) const
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
inline Bitboard MagicBase::attack_bb<PieceType::CANNON>(Square s, Bitboard occupies) const
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
inline Bitboard MagicBase::attack_bb<PieceType::KNIGHT>(Square s, Bitboard occupies) const
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
inline Bitboard MagicBase::attack_bb<PieceType::KNIGHT_TO>(Square s, Bitboard occupies) const
{
    Bitboard result = 0;
    constexpr int forwards[8] = {2 * SQ_NORTH + SQ_EAST, 2 * SQ_EAST + SQ_NORTH, 
                                 2 * SQ_EAST + SQ_SOUTH, 2 * SQ_SOUTH + SQ_EAST, 
                                 2 * SQ_SOUTH + SQ_WEST, 2 * SQ_WEST + SQ_SOUTH, 
                                 2 * SQ_WEST + SQ_NORTH, 2 * SQ_NORTH + SQ_WEST}; 
    constexpr int blocks[4] = {SQ_NORTH + SQ_EAST, SQ_EAST + SQ_SOUTH, SQ_SOUTH + SQ_WEST, SQ_WEST + SQ_NORTH};
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
inline Bitboard MagicBase::attack_bb<PieceType::BISHOP>(Square s, Bitboard occupies) const
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

#endif // MAGIC_BASE_H