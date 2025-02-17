#ifndef EVALUATE_H
#define EVALUATE_H

#include <vector>

#include "position.h"
#include "bitboard.h"


constexpr Value Infinite = 30000;
constexpr Value MateValue = 20000;
constexpr Value RookValue = 1600;
constexpr Value KnightValue = 750;
constexpr Value BishopValue = 150;
constexpr Value AdvisorValue = 150;
constexpr Value KingValue = MateValue;
constexpr Value CannonValue = 760;  // Knight leave initial position will increase 10 score
constexpr Value PawnValue = 100;

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
    Value value = 0;
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
    static Value Eval(const Position& position);
private:
    template <PieceType pt>
    static Value ScoreCalculator(const Position& position)
    {
        return BB1Count(position.Pieces(Color::RED, pt)) * PieceValue<pt> - BB1Count(position.Pieces(Color::BLACK, pt)) * PieceValue<pt>;
    }
};
// Pawn that across to river gain double score, 
// gain more score near enemy king(prefer vertical)
// gain more score side by side with other friend pawn
template <>
inline Value Evaluate::ScoreCalculator<PieceType::PAWN>(const Position& position)
{
    Value score = 0;
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
        score += (2 * v + h);
        blackPawnSquares.push_back(ps);
    }
    // set first pawn as origin
    int blackDistance = 0;
    for (size_t i = 1; i < redPawnSquares.size(); ++i)
    {
        blackDistance += Distance(blackPawnSquares[0], blackPawnSquares[i]);
    }
    score += blackDistance;

    return ((BB1Count(redPawnBB & BlackRegion) * 2 + BB1Count(redPawnBB & RedRegion)) - (BB1Count(blackPawnBB & RedRegion) * 2 + BB1Count(blackPawnBB & BlackRegion)))
           * PieceValue<PieceType::PAWN>;
}

#endif