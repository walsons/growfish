#include "evaluate.h"

int Evaluate::Eval(const Position& position)
{
    static_assert(NUM(PieceType::PAWN) == 0 && NUM(PieceType::KING) == 6, "piece type value changed");

    int score = 0;
    score += ScoreCalculator<PieceType::PAWN>(position);
    score += ScoreCalculator<PieceType::ADVISOR>(position);
    score += ScoreCalculator<PieceType::BISHOP>(position);
    score += ScoreCalculator<PieceType::KNIGHT>(position);
    score += ScoreCalculator<PieceType::CANNON>(position);
    score += ScoreCalculator<PieceType::ROOK>(position);
    score += ScoreCalculator<PieceType::KING>(position);
    score += OneCount(position.Pieces(Color::RED) & BlackRegin) * 10;
    score -= OneCount(position.Pieces(Color::BLACK) & RedRegion) * 10;

    return position.side_to_move() == Color::RED ? score : -score;
}
