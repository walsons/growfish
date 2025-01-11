#ifndef TYPES_H
#define TYPES_H

#if defined(_MSC_VER)
#include <__msvc_int128.hpp>
using __uint128_t = std::_Unsigned128;
#endif

#include <cassert>
#include <algorithm>

using Bitboard = __uint128_t;

using Value = short;
using Ply = Value;  // Ply often used to do addition and subtraction with Value
using Depth = unsigned char;

enum File : char
{
    FILE_A,
    FILE_B,
    FILE_C,
    FILE_D,
    FILE_E,
    FILE_F,
    FILE_G,
    FILE_H,
    FILE_I,
    FILE_NB
};

enum Rank : char
{
    RANK_0,
    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_9,
    RANK_NB
};

enum Square : char
{
    SQ_A0, SQ_B0, SQ_C0, SQ_D0, SQ_E0, SQ_F0, SQ_G0, SQ_H0, SQ_I0,
    SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1, SQ_I1,
    SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2, SQ_I2,
    SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3, SQ_I3,
    SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4, SQ_I4,
    SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5, SQ_I5,
    SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6, SQ_I6,
    SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7, SQ_I7,
    SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8, SQ_I8,
    SQ_A9, SQ_B9, SQ_C9, SQ_D9, SQ_E9, SQ_F9, SQ_G9, SQ_H9, SQ_I9,
    SQ_NUM
};

inline bool LegalSquare(Square s) { return s >= SQ_A0 && s < SQ_NUM; }

inline Square NorthEnd(Square s) { return SQ_NUM; }
inline Square SouthEnd(Square s) { return static_cast<Square>(SQ_A0 - 1); }
inline Square EastEnd(Square s) { return static_cast<Square>(static_cast<char>(s) / 9 * 9 + 9); }
inline Square WestEnd(Square s) { return static_cast<Square>(static_cast<char>(s) / 9 * 9 - 1); }

inline bool IsNotNorthEnd(Square current, Square start) { return current < NorthEnd(start); }
inline bool IsNotSouthEnd(Square current, Square start) { return current > SouthEnd(start); }
inline bool IsNotEastEnd(Square current, Square start) { return current < EastEnd(start); }
inline bool IsNotWestEnd(Square current, Square start) { return current > WestEnd(start); }

inline File FileOf(Square s) {
    return static_cast<File>(s % FILE_NB);
}
inline Rank RankOf(Square s) {
    return static_cast<Rank>(s / FILE_NB);
}


enum Direction : char
{
    SQ_NORTH = 9,
    SQ_SOUTH = -SQ_NORTH,
    SQ_EAST = 1,
    SQ_WEST = -SQ_EAST
};

inline constexpr Direction operator*(int x, Direction d)
{
    return static_cast<Direction>(x * static_cast<char>(d));
}

inline Square operator+(Square sq, Direction d)
{
    return static_cast<Square>(static_cast<char>(sq) + static_cast<char>(d));
}

inline Square& operator+=(Square& sq, Direction d)
{
    sq = static_cast<Square>(static_cast<char>(sq) + static_cast<char>(d));
    return sq;
}


// template function to use enum class as a int
template <typename E>
constexpr int NUM(E e) {
    return static_cast<int>(e);
}

enum class PieceType : char
{
    PAWN,
    ADVISOR,
    BISHOP,
    KNIGHT,
    CANNON,
    ROOK,
    KING,

    NO_PIECE_TYPE,
    
    PIECE_TYPE_NUM,

    // A special case
    KNIGHT_TO,
    PAWN_TO
};

enum class Piece : char
{
    W_PAWN,
    W_ADVISOR,
    W_BISHOP,
    W_KNIGHT,
    W_CANNON,
    W_ROOK,
    W_KING,

    B_PAWN,
    B_ADVISOR,
    B_BISHOP,
    B_KNIGHT,
    B_CANNON,
    B_ROOK,
    B_KING,

    NO_PIECE,

    PIECE_NUM, 

    START = W_PAWN,
    END = B_KING,
    W_START = W_PAWN,
    W_END = W_KING,
    B_START = B_PAWN,
    B_END = B_KING
};

inline PieceType TypeOfPiece(Piece p)
{
    assert(p < Piece::PIECE_NUM);
    if (p == Piece::NO_PIECE)
        return PieceType::NO_PIECE_TYPE;
    return static_cast<PieceType>(p < Piece::B_START ? NUM(p) : (NUM(p) - NUM(Piece::B_START)));
}

enum class Color : char
{
    RED,
    BLACK,
    // NO_COLOR,
    COLOR_NUM
};

inline constexpr Color operator!(Color c)
{
    return c == Color::RED ? Color::BLACK : Color::RED;
}

inline Color ColorOfPiece(Piece p)
{
    assert((p < Piece::PIECE_NUM && p != Piece::NO_PIECE));
    // if (p == Piece::NO_PIECE)
    //     return Color::NO_COLOR;
    return static_cast<Color>(p < Piece::B_START ? Color::RED : Color::BLACK);
}

using U64 = unsigned long long;

// Used to detemine relative position for Knight and Bishop
inline char Distance(Square s1, Square s2) 
{
    int v = std::abs(s1 / 9 - s2 / 9);
    int h = std::abs(s1 % 9 - s2 % 9);
    return std::max(v, h);
}


#endif