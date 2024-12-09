#ifndef EVALUATE_H
#define EVALUATE_H

#include "position.h"
#include "bitboard.h"

// pikafish piece value
//constexpr int RookValue = 1213;
//constexpr int KnightValue = 964;
//constexpr int BishopValue = 191;
//constexpr int AdvisorValue = 216;
//constexpr int KingValue = MAXS;
//constexpr int CannonValue = 746;
//constexpr int PawnValue = 140;

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
template <> constexpr int PieceValue<PieceType::ROOK> = RookValue;
template <> constexpr int PieceValue<PieceType::KNIGHT> = KnightValue;
template <> constexpr int PieceValue<PieceType::BISHOP> = BishopValue;
template <> constexpr int PieceValue<PieceType::ADVISOR> = AdvisorValue;
template <> constexpr int PieceValue<PieceType::KING> = KingValue;
template <> constexpr int PieceValue<PieceType::CANNON> = CannonValue;
template <> constexpr int PieceValue<PieceType::PAWN> = PawnValue;

inline int GetPieceValue(PieceType pt)
{
    int value = 0;
    switch (pt)
    {
    case PieceType::ROOK:
        value = PieceValue<PieceType::ROOK>;
        break;
    case PieceType::KNIGHT:
        value = PieceValue<PieceType::KNIGHT>;
        break;
    case PieceType::BISHOP:
        value = PieceValue<PieceType::BISHOP>;
        break;
    case PieceType::ADVISOR:
        value = PieceValue<PieceType::ADVISOR>;
        break;
    case PieceType::KING:
        value = PieceValue<PieceType::KING>;
        break;
    case PieceType::CANNON:
        value = PieceValue<PieceType::CANNON>;
        break;
    case PieceType::PAWN:
        value = PieceValue<PieceType::PAWN>;
        break;
    default:
        break;
    }
    return value;
}

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