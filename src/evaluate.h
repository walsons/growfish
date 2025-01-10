#ifndef EVALUATE_H
#define EVALUATE_H

#include <vector>

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

constexpr int Infinite = 30000;
constexpr int MateValue = 20000;
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
// Pawn that across to river gain double score, 
// gain more score near enemy king(prefer vertical)
// gain more score side by side with other friend pawn
template <>
inline int Evaluate::ScoreCalculator<PieceType::PAWN>(const Position& position)
{
    int score = 0;
    Bitboard redPawnBB = position.Pieces(Color::RED, PieceType::PAWN);
    Bitboard blackPawnBB = position.Pieces(Color::BLACK, PieceType::PAWN);

    std::vector<Square> redPawnSquares;
    redPawnSquares.reserve(5);
    auto tmp = redPawnBB;
    while (tmp)
    {
        Square ps = PopLSB(tmp);
        Square ks = position.KingSquare(Color::BLACK);
        int v = std::abs(ps / 9 - ks / 9);
        int h = std::abs(ps % 9 - ks % 9);
        score -= (2 * v + h);
        redPawnSquares.push_back(ps);
    }
    // set first pawn as origin
    int redDistance = 0;
    for (size_t i = 1; i < redPawnSquares.size(); ++i)
    {
        redDistance += Distance(redPawnSquares[0], redPawnSquares[i]);
    }
    score -= redDistance;

    std::vector<Square> blackPawnSquares;
    blackPawnSquares.reserve(5);
    tmp = blackPawnBB;
    while (tmp)
    {
        Square ps = PopLSB(tmp);
        Square ks = position.KingSquare(Color::RED);
        int v = std::abs(ps / 9 - ks / 9);
        int h = std::abs(ps % 9 - ks % 9);
        score += 2 * (10 - v) + (10 - h);
        blackPawnSquares.push_back(ps);
    }
    // set first pawn as origin
    int blackDistance = 0;
    for (size_t i = 1; i < redPawnSquares.size(); ++i)
    {
        blackDistance += Distance(blackPawnSquares[0], blackPawnSquares[i]);
    }
    score += blackDistance;

    return ((OneCount(redPawnBB & BlackRegion) * 2 + OneCount(redPawnBB & RedRegion)) - (OneCount(blackPawnBB & RedRegion) * 2 + OneCount(blackPawnBB & BlackRegion)))
           * PieceValue<PieceType::PAWN>;
}

#endif