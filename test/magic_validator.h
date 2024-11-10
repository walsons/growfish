#ifndef MAGIC_VALIDATOR_H
#define MAGIC_VALIDATOR_H

#include <vector>

#include "src/bitboard.h"
#include "src/position.h"
#include "src/function.h"

/* This class aims to validate if the generated magic is correct.
*  It generate pseudo legal moves(rook, cannon, knight, bishop) for a specified position by move piece in
*  board, and compare with generated pseudo legal moves by bitboard.
*/ 

class MagicValidator
{
public:
    MagicValidator(const Position& position, 
                   const std::vector<Move>& magic_rook_moves,
                   const std::vector<Move>& magic_cannon_moves,
                   const std::vector<Move>& magic_knight_moves,
                   const std::vector<Move>& magic_bishop_moves)
        : position_(position)
        , magic_rook_moves_(magic_rook_moves)
        , magic_cannon_moves_(magic_cannon_moves)
        , magic_knight_moves_(magic_knight_moves)
        , magic_bishop_moves_(magic_bishop_moves)
    {
    }

    void Validate() const
    {
        // TODO
    }
    
private:
    template <Color c>
    void RookMove(Square s)
    {
        for (Square pos = s + SQ_NORTH; pos < NorthEnd(s); pos += SQ_NORTH)
        {
            if (IsColor<c>(position_.piece_from_square(pos)))
                break;
            pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, pos));
            if (!IsEmpty(position_.piece_from_square(pos)))
            {
                pseudo_legal_non_capture_moves_.pop_back();
                pseudo_legal_capture_moves_.push_back(ConstructMove(s, pos));
                break;
            }
        }
        for (Square pos = s + SQ_SOUTH; pos > SouthEnd(s); pos += SQ_SOUTH)
        {
            if (IsColor<c>(position_.piece_from_square(pos)))
                break;
            pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, pos));
            if (!IsEmpty(position_.piece_from_square(pos)))
            {
                pseudo_legal_non_capture_moves_.pop_back();
                pseudo_legal_capture_moves_.push_back(ConstructMove(s, pos));
                break;
            }
        }
        for (Square pos = s + SQ_EAST; pos < EastEnd(s); pos += SQ_EAST)
        {
            if (IsColor<c>(position_.piece_from_square(pos)))
                break;
            pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, pos));
            if (!IsEmpty(position_.piece_from_square(pos)))
            {
                pseudo_legal_non_capture_moves_.pop_back();
                pseudo_legal_capture_moves_.push_back(ConstructMove(s, pos));
                break;
            }
        }
        for (Square pos = s + SQ_WEST; pos > WestEnd(s); pos += SQ_WEST)
        {
            if (IsColor<c>(position_.piece_from_square(pos)))
                break;
            pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, pos));
            if (!IsEmpty(position_.piece_from_square(pos)))
            {
                pseudo_legal_non_capture_moves_.pop_back();
                pseudo_legal_capture_moves_.push_back(ConstructMove(s, pos));
                break;
            }
        }
    }

    template <Color c>
    void CannonMove(Square s)
    {
        auto f = [&](Direction d, decltype(IsNotNorthEnd) f) {
            bool flag = false;
            for (Square pos = s + d; f(pos, s); pos += d)
            {
                if (position_.piece_from_square(pos) != Piece::NO_PIECE)
                {
                    if (flag)
                    {
                        if (IsColor<!c>(position_.piece_from_square(pos)))
                            pseudo_legal_capture_moves_.push_back(ConstructMove(s, pos));
                        break;
                    }
                    else
                    {
                        flag = true;
                        continue;
                    }
                }
                else
                {
                    if (flag)
                    {
                        continue;
                    }
                    else
                    {
                        pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, pos));
                    }
                }
            }
        };
        f(SQ_NORTH, IsNotNorthEnd);
        f(SQ_SOUTH, IsNotSouthEnd);
        f(SQ_EAST, IsNotEastEnd);
        f(SQ_WEST, IsNotWestEnd);
    }

    template <Color c>
    void KnightMove(Square s)
    {
        auto f = [&](Square barrier, Square destination1, Square destination2) {
            if (SQ_A0 <= barrier && barrier < SQ_NUM && Distance(s, barrier) == 1 && position_.piece_from_square(barrier) == Piece::NO_PIECE)
            {
                if (SQ_A0 <= destination1 && destination1 < SQ_NUM && Distance(s, destination1) == 2)
                {
                    if (IsEmpty(position_.piece_from_square(destination1)))
                        pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, destination1));
                    else if (IsColor<!c>(position_.piece_from_square(destination1)))
                        pseudo_legal_capture_moves_.push_back(ConstructMove(s, destination1));
                }
                if (SQ_A0 <= destination2 && destination2 < SQ_NUM && Distance(s, destination2) == 2)
                {
                    if (IsEmpty(position_.piece_from_square(destination1)))
                        pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, destination2));
                    else if (IsColor<!c>(position_.piece_from_square(destination1)))
                        pseudo_legal_capture_moves_.push_back(ConstructMove(s, destination2));
                }
            }
        };
        // North
        static constexpr Direction N2E1 = static_cast<Direction>(SQ_NORTH + SQ_NORTH + SQ_EAST);
        static constexpr Direction N2W1 = static_cast<Direction>(SQ_NORTH + SQ_NORTH + SQ_WEST);
        f(s + SQ_NORTH, s + N2E1, s + N2W1);
        // South
        static constexpr Direction S2E1 = static_cast<Direction>(SQ_SOUTH + SQ_SOUTH + SQ_EAST);
        static constexpr Direction S2W1 = static_cast<Direction>(SQ_SOUTH + SQ_SOUTH + SQ_WEST);
        f(s + SQ_SOUTH, s + S2E1, s + S2W1);
        // East
        static constexpr Direction E2N1 = static_cast<Direction>(SQ_EAST + SQ_EAST + SQ_NORTH);
        static constexpr Direction E2S1 = static_cast<Direction>(SQ_EAST + SQ_EAST + SQ_SOUTH);
        f(s + SQ_EAST, s + E2N1, s + E2S1);
        // West
        static constexpr Direction W2N1 = static_cast<Direction>(SQ_WEST + SQ_WEST + SQ_NORTH);
        static constexpr Direction W2S1 = static_cast<Direction>(SQ_WEST + SQ_WEST + SQ_SOUTH);
        f(s + SQ_WEST, s + W2N1, s + W2S1);
    }

    template <Color c>
    void BishopMove(Square s)
    {
        auto f = [&](Square barrier, Square destination) {
            static constexpr Square SQ_BEG = (c == Color::RED ? SQ_A0 : SQ_A5);
            static constexpr Square SQ_END = (c == Color::RED ? SQ_A5 : SQ_NUM);
            if (SQ_BEG <= barrier && barrier < SQ_END && Distance(s, barrier) == 1 && position_.piece_from_square(barrier) == Piece::NO_PIECE)
            {
                if (SQ_BEG <= destination && destination < SQ_END && Distance(s, destination) == 2)
                {
                    if (IsEmpty(position_.piece_from_square(destination)))
                        pseudo_legal_non_capture_moves_.push_back(ConstructMove(s, destination));
                    else if (IsColor<!c>(position_.piece_from_square(destination)))
                        pseudo_legal_capture_moves_.push_back(ConstructMove(s, destination));
                }
            }
        };
        // North East
        static constexpr Direction N1E1 = static_cast<Direction>(SQ_NORTH + SQ_EAST);
        static constexpr Direction N2E2 = static_cast<Direction>(SQ_NORTH + SQ_NORTH + SQ_EAST + SQ_EAST);
        f(s + N1E1, s + N2E2);
        // North West
        static constexpr Direction N1W1 = static_cast<Direction>(SQ_NORTH + SQ_WEST);
        static constexpr Direction N2W2 = static_cast<Direction>(SQ_NORTH + SQ_NORTH + SQ_WEST + SQ_WEST);
        f(s + N1W1, s + N2W2);
        // South East
        static constexpr Direction S1E1 = static_cast<Direction>(SQ_SOUTH + SQ_EAST);
        static constexpr Direction S2E2 = static_cast<Direction>(SQ_SOUTH + SQ_SOUTH + SQ_EAST + SQ_EAST);
        f(s + S1E1, s + S2E2);
        // South West
        static constexpr Direction S1W1 = static_cast<Direction>(SQ_SOUTH + SQ_WEST);
        static constexpr Direction S2W2 = static_cast<Direction>(SQ_SOUTH + SQ_SOUTH + SQ_WEST + SQ_WEST);
        f(s + S1W1, s + S2W2);
    }

private:
    Position position_;

    std::vector<Move> magic_rook_moves_;
    std::vector<Move> magic_cannon_moves_;
    std::vector<Move> magic_knight_moves_;
    std::vector<Move> magic_bishop_moves_;
};

#endif