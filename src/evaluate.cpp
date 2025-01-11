#include "evaluate.h"


Value Evaluate::Eval(const Position& position)
{
    static_assert(NUM(PieceType::PAWN) == 0 && NUM(PieceType::KING) == 6, "piece type value changed");

    Value score = 0;
    score += ScoreCalculator<PieceType::PAWN>(position);
    score += ScoreCalculator<PieceType::ADVISOR>(position);
    score += ScoreCalculator<PieceType::BISHOP>(position);
    score += ScoreCalculator<PieceType::KNIGHT>(position);
    score += ScoreCalculator<PieceType::CANNON>(position);
    score += ScoreCalculator<PieceType::ROOK>(position);
    score += ScoreCalculator<PieceType::KING>(position);

    // Addition score
    // Piece in enemy region gain extra 10 score
    score += OneCount(position.Pieces(Color::RED) & BlackRegion) * 10;
    score -= OneCount(position.Pieces(Color::BLACK) & RedRegion) * 10;

    // In order to encourage knight to move from initial position
    if (position.piece_count() > 26)
    {
        if (position.piece_from_square(SQ_B0) != Piece::W_KNIGHT)
            score += 10;
        if (position.piece_from_square(SQ_H0) != Piece::W_KNIGHT)
            score += 10;
        if (position.piece_from_square(SQ_B9) != Piece::B_KNIGHT)
            score -= 10;
        if (position.piece_from_square(SQ_H9) != Piece::B_KNIGHT)
            score -= 10;
    }

    return position.side_to_move() == Color::RED ? score : -score;
}
