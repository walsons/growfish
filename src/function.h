#ifndef FUNCTION_H
#define FUNCTION_H

#include "structures.h"

constexpr unsigned char PieceIndex(const Piece& piece) { return static_cast<unsigned char>(piece); }

inline Move ConstructMove(Square from, Square to)
{
    return ((0x00FF & from) << 8) | to;
}

inline Square MoveFrom(Move move)
{
    return static_cast<Square>((move & 0xFF00) >> 8);
}

inline Square MoveTo(Move move)
{
    return static_cast<Square>(move & 0x00FF);
}

inline bool IsEmpty(Piece piece) { return piece == Piece::NO_PIECE; }

template <Color c>
bool IsColor(Piece piece);
template <>
inline bool IsColor<Color::RED>(Piece piece) { return piece >= Piece::W_START && piece <= Piece::W_END; }
template <>
inline bool IsColor<Color::BLACK>(Piece piece) { return piece >= Piece::B_START && piece <= Piece::B_END; }

template <Color c>
bool IsInPalace(Square s);
template <>
inline bool IsInPalace<Color::RED>(Square s)
{
    return (SQ_D0 <= s && s <= SQ_F0) || (SQ_D1 <= s && s <= SQ_F1) || (SQ_D2 <= s && s <= SQ_F2);
}
template <>
inline bool IsInPalace<Color::BLACK>(Square s)
{
    return (SQ_D9 <= s && s <= SQ_F9) || (SQ_D8 <= s && s <= SQ_F8) || (SQ_D7 <= s && s <= SQ_F7);
}

inline std::string Move2String(Move move)
{
    std::string str;
    Square from = MoveFrom(move);
    Square to = MoveTo(move);
    char s1 = static_cast<char>(from % 9 + 'a');
    int i1 = from / 9;
    char s2 = static_cast<char>(to % 9 + 'a');
    int i2 = to / 9;
    str += std::string(1, s1);
    str += std::to_string(i1);
    str += std::string(1, s2);
    str += std::to_string(i2);
    return str;
}

inline Move String2Move(std::string str)
{
    Square from = static_cast<Square>((str[0] - 'a') + (str[1] - '0') * 9);
    Square to = static_cast<Square>((str[2] - 'a') + (str[3] - '0') * 9);
    return ConstructMove(from, to);
}

#endif