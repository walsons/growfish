#include "evaluate.h"

constexpr int GameStartRookMoveBonus[90] = {
    0, 20, 0, 0, 0, 0, 0, 20, 0,
    10, 20, 0, 0, 0, 0, 0, 20, 10,
    10, 20, 0, 0, 0, 0, 0, 20, 10,
    0, 20, 0, 0, 0, 0, 0, 20, 0,
    0, 30, 0, 50, 0, 50, 0, 30, 0,
    0, 30, 0, 50, 0, 50, 0, 30, 0,
    0, 20, 0, 0, 0, 0, 0, 20, 0,
    10, 20, 0, 0, 0, 0, 0, 20, 10,
    10, 20, 0, 0, 0, 0, 0, 20, 10,
    0, 20, 0, 0, 0, 0, 0, 20, 0,
};

constexpr int GameStartKnightMoveBonus[90] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    10, 0, 10, 0, 0, 0, 10, 0, 10,
    0, 0, 0, 0, 20, 0, 0, 0, 0,
    0, 10, 0, 20, 0, 20, 0, 1, 0,
    0, 10, 0, 20, 0, 20, 0, 1, 0,
    0, 0, 0, 0, 20, 0, 0, 0, 0,
    10, 0, 10, 0, 0, 0, 10, 0, 10,
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
    // Piece in enemy region gain extra 10 score
    score += OneCount(position.Pieces(Color::RED) & BlackRegion) * 10;
    score -= OneCount(position.Pieces(Color::BLACK) & RedRegion) * 10;
    // Enemy has two advisor, increase own cannon value
    // if (OneCount(position.Pieces(Color::BLACK, PieceType::ADVISOR)) == 2)
    //     score += OneCount(position.Pieces(Color::RED, PieceType::CANNON) * 30);
    // if (OneCount(position.Pieces(Color::RED, PieceType::ADVISOR)) == 2)
    //     score += OneCount(position.Pieces(Color::BLACK, PieceType::CANNON) * 30);

    // In order to encourage rook and knight move forward
    // if (position.piece_count() > 28)
    // {
    //     Bitboard rbb = position.Pieces(Color::RED, PieceType::ROOK);
    //     while (rbb)
    //     {
    //         score += GameStartRookMoveBonus[NUM(PopLSB(rbb))];
    //     }
    //     rbb = position.Pieces(Color::BLACK, PieceType::ROOK);
    //     while (rbb)
    //     {
    //         score -= GameStartRookMoveBonus[NUM(PopLSB(rbb))];
    //     }
    //     Bitboard nbb = position.Pieces(Color::RED, PieceType::KNIGHT);
    //     while (nbb)
    //     {
    //         score += GameStartKnightMoveBonus[NUM(PopLSB(nbb))];
    //     }
    //     nbb = position.Pieces(Color::BLACK, PieceType::KNIGHT);
    //     while (nbb)
    //     {
    //         score -= GameStartKnightMoveBonus[NUM(PopLSB(nbb))];
    //     }
    // }


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
