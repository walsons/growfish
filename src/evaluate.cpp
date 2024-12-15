#include "evaluate.h"

constexpr int GameStartRookMoveBonus[90] = {
    0, 2, 0, 0, 0, 0, 0, 2, 0,
    1, 2, 0, 0, 0, 0, 0, 2, 1,
    1, 2, 0, 0, 0, 0, 0, 2, 1,
    0, 2, 0, 0, 0, 0, 0, 2, 0,
    0, 5, 0, 5, 0, 5, 0, 5, 0,
    0, 5, 0, 5, 0, 5, 0, 5, 0,
    0, 2, 0, 0, 0, 0, 0, 2, 0,
    1, 2, 0, 0, 0, 0, 0, 2, 1,
    1, 2, 0, 0, 0, 0, 0, 2, 1,
    0, 2, 0, 0, 0, 0, 0, 2, 0,
};

constexpr int GameStartKnightMoveBonus[90] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 1, 0, 0, 0, 1, 0, 1,
    0, 0, 0, 0, 2, 0, 0, 0, 0,
    0, 1, 0, 2, 0, 2, 0, 1, 0,
    0, 1, 0, 2, 0, 2, 0, 1, 0,
    0, 0, 0, 0, 2, 0, 0, 0, 0,
    1, 0, 1, 0, 0, 0, 1, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};

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

    // Addition score
    // 1. Piece in enemy region gain extra 10 score
    score += OneCount(position.Pieces(Color::RED) & BlackRegion) * 10;
    score -= OneCount(position.Pieces(Color::BLACK) & RedRegion) * 10;
    // 2. In order to encourage rook and knight move forward
    // TODO...
    // 3. Cannon and enemy king in the same line, gain extra 50 score
    // Bitboard redCannonB = position.Pieces(Color::RED, PieceType::CANNON);
    // Bitboard blackKingB = position.Pieces(Color::BLACK, PieceType::KING);
    // Square ks = PopLSB(blackKingB);
    // File kf = FileOf(ks);
    // Rank kr = RankOf(ks);
    // while (redCannonB)
    // {
    //     Square s = PopLSB(redCannonB);
    //     if (FileOf(s) == kf || RankOf(s) == kr)
    //     {
    //         score += 50;
    //         break;
    //     }
    // }
    // Bitboard blackCannonB = position.Pieces(Color::BLACK, PieceType::CANNON);
    // Bitboard redKingB = position.Pieces(Color::RED, PieceType::KING);
    // ks = PopLSB(redKingB);
    // kf = FileOf(ks);
    // kr = RankOf(ks);
    // while (blackCannonB)
    // {
    //     Square s = PopLSB(blackCannonB);
    //     if (FileOf(s) == kf || RankOf(s) == kr)
    //     {
    //         score -= 50;
    //         break;
    //     }
    // }

    return position.side_to_move() == Color::RED ? score : -score;
}
