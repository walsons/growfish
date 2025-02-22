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
    score += BB1Count(position.Pieces(Color::RED) & BlackRegion) * 10;
    score -= BB1Count(position.Pieces(Color::BLACK) & RedRegion) * 10;

    // Punish if Rook and Knight are at initial position at the same time
    if (position.piece_count() > 20)
    {
        if (   position.piece_from_square(SQ_A0) == Piece::W_ROOK
            && position.piece_from_square(SQ_B0) == Piece::W_KNIGHT)
            score -= 200;
        if (   position.piece_from_square(SQ_I0) == Piece::W_ROOK
            && position.piece_from_square(SQ_H0) == Piece::W_KNIGHT)
            score -= 200;
        if (   position.piece_from_square(SQ_A9) == Piece::B_ROOK
            && position.piece_from_square(SQ_B9) == Piece::B_KNIGHT)
            score += 200;
        if (   position.piece_from_square(SQ_I9) == Piece::B_ROOK
            && position.piece_from_square(SQ_H9) == Piece::B_KNIGHT)
            score += 200;
    }

    // Encourage Bishop protect one other
    auto bishopBonus = [&](Color c) {
        Bitboard bishopBB = 0;
        if ((bishopBB = position.Pieces(c, PieceType::BISHOP)))
        {
            Square b1 = PopLSB(bishopBB);
            if (bishopBB)
            {
                Square b2 = PopLSB(bishopBB);
                if (Distance(b1, b2) <= 2)
                {
                    score = (c == Color::RED ? score + 20 : score - 20);
                }
            }
        }
    };
    bishopBonus(Color::RED);
    bishopBonus(Color::BLACK);

    return position.side_to_move() == Color::RED ? score : -score;
}
