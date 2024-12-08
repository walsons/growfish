#ifndef EVALUATE_H
#define EVALUATE_H

#include "position.h"
#include "bitboard.h"

constexpr int Infinite = 60000;
constexpr int MateValue = 50000;
constexpr int RookValue = 1600;
constexpr int KnightValue = 750;
constexpr int BishopValue = 150;
constexpr int AdvisorValue = 150;
constexpr int KingValue = MateValue;
constexpr int CannonValue = 750;
constexpr int PawnValue = 100;
template <PieceType pt> constexpr int PieceValue;
template <> constexpr int PieceValue<PieceType::ROOK> = 1600;
template <> constexpr int PieceValue<PieceType::KNIGHT> = 750;
template <> constexpr int PieceValue<PieceType::BISHOP> = 150;
template <> constexpr int PieceValue<PieceType::ADVISOR> = 150;
template <> constexpr int PieceValue<PieceType::KING> = MateValue;
template <> constexpr int PieceValue<PieceType::CANNON> = 750;
template <> constexpr int PieceValue<PieceType::PAWN> = 100;

// pikafish piece value
//constexpr int RookValue = 1213;
//constexpr int KnightValue = 964;
//constexpr int BishopValue = 191;
//constexpr int AdvisorValue = 216;
//constexpr int KingValue = MAXS;
//constexpr int CannonValue = 746;
//constexpr int PawnValue = 140;

class Evaluate
{
public:
    static int Eval(const Position& position);
private:
    template <PieceType pt>
    static int ScoreCalculator(const Position& position)
    {
        return OneCount(position.Pieces(Color::RED, pt)) * PieceValue<pt> - OneCount(position.Pieces(Color::BLACK, pt)) * PieceValue<pt>;
    }
};

#endif