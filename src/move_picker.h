#ifndef MOVE_PICKER_H
#define MOVE_PICKER_H

#include <list>
#include "move_generator.h"
#include "history.h"
#include "evaluate.h"

// MVV-LVA
/*
    attacker \ victim  P    A    B    N    C    R    K    p    a    b    n    c    r    k
    ---------------------------------------------------------------------------------------
       P              107  207  307  407  507  607  707  107  207  307  407  507  607  707
       A              106  206  306  406  506  606  706  106  206  306  406  506  606  706
       B              105  205  305  405  505  605  705  105  205  305  405  505  605  705
       N              104  204  304  404  504  604  704  104  204  304  404  504  604  704 
       C              103  203  303  403  503  603  703  103  203  303  403  503  603  703
       R              102  202  302  402  502  602  702  102  202  302  402  502  602  702
       K              101  201  301  401  501  601  701  101  201  301  401  501  601  701
       p              107  207  307  407  507  607  707  107  207  307  407  507  607  707
       a              106  206  306  406  506  606  706  106  206  306  406  506  606  706
       b              105  205  305  405  505  605  705  105  205  305  405  505  605  705
       n              104  204  304  404  504  604  704  104  204  304  404  504  604  704 
       c              103  203  303  403  503  603  703  103  203  303  403  503  603  703
       r              102  202  302  402  502  602  702  102  202  302  402  502  602  702
       k              101  201  301  401  501  601  701  101  201  301  401  501  601  701

*/
const Value MVV_LVA[14][14] = {
    { 107, 207, 307, 407, 507, 607, 707, 107, 207, 307, 407, 507, 607, 707 },
    { 106, 206, 306, 406, 506, 606, 706, 106, 206, 306, 406, 506, 606, 706 },
    { 105, 205, 305, 405, 505, 605, 705, 105, 205, 305, 405, 505, 605, 705 },
    { 104, 204, 304, 404, 504, 604, 704, 104, 204, 304, 404, 504, 604, 704 },
    { 103, 203, 303, 403, 503, 603, 703, 103, 203, 303, 403, 503, 603, 703 },
    { 102, 202, 302, 402, 502, 602, 702, 102, 202, 302, 402, 502, 602, 702 },
    { 101, 201, 301, 401, 501, 601, 701, 101, 201, 301, 401, 501, 601, 701 },
    { 107, 207, 307, 407, 507, 607, 707, 107, 207, 307, 407, 507, 607, 707 },
    { 106, 206, 306, 406, 506, 606, 706, 106, 206, 306, 406, 506, 606, 706 },
    { 105, 205, 305, 405, 505, 605, 705, 105, 205, 305, 405, 505, 605, 705 },
    { 104, 204, 304, 404, 504, 604, 704, 104, 204, 304, 404, 504, 604, 704 },
    { 103, 203, 303, 403, 503, 603, 703, 103, 203, 303, 403, 503, 603, 703 },
    { 102, 202, 302, 402, 502, 602, 702, 102, 202, 302, 402, 502, 602, 702 },
    { 101, 201, 301, 401, 501, 601, 701, 101, 201, 301, 401, 501, 601, 701 }
};

struct ScoreMove
{
    Move move;
    Value score;
};

class MovePicker
{
public:
    enum class Phase 
    {
        TT,
        CHECK,
        CAPTURE,
        KILLER,
        NON_CAPTURE,
        BAD_CAPTURE,
        EVASION,
        QSEARCH_CAPTURE,
        END,
    };

    MovePicker(size_t threadIndex, const Position& position, Move ttMove, Move killerMove[], Phase phase = Phase::TT);
    Move NextMove();

    Value SEECapture(Color c, Move captureMove)
    {
        return c == Color::RED ? SEECapture<Color::RED>(captureMove)
                               : SEECapture<Color::BLACK>(captureMove);
    }

private:
    std::list<ScoreMove> GenerateCheckMove();
    std::list<ScoreMove> GenerateCaptureMove();
    std::list<ScoreMove> GenerateCaptureMove_only_mvv_lva();
    std::list<ScoreMove> GenerateNonCaptureMove();
    std::list<ScoreMove> GenerateEvasionMove();

    template <Color c>
    Value SEE(Piece victim, Square to, Bitboard &swapPiecesBB);
    template <Color c>
    Value SEECapture(Move captureMove);

private:
    size_t thread_index_;

    Position position_;
    Move tt_move_;
    Move killer_move1_;
    Move killer_move2_;
    MoveGenerator move_generator_;
    Phase phase_;
    std::list<ScoreMove> moves_;
    std::list<ScoreMove> bad_captures_;
};

template <Color c>
Value MovePicker::SEE(Piece victim, Square to, Bitboard &swapPiecesBB)
{
    auto makeCapture = [this, victim, to, &swapPiecesBB](Bitboard attack, Value &value) {
        if (attack)
        {
            Square from = PopLSB(attack);
            swapPiecesBB |= SquareBB(from);
            Piece attacker = position_.piece_from_square(from);
            // attacker become victim in next SEE
            value = GetPieceValue(TypeOfPiece(victim)) - SEE<!c>(attacker, to, swapPiecesBB);
            value = value > 0 ? value : 0;
            return true;
        }
        return false;
    };

    Value value = 0;

    Bitboard attack = AttackBB<PieceType::PAWN_TO>(to, c);
    attack &= position_.Pieces(c, PieceType::PAWN);
    attack &= ~swapPiecesBB;
    if (makeCapture(attack, value))
        return value;
    
    attack = AttackBB<PieceType::ADVISOR>(to);
    attack &= position_.Pieces(c, PieceType::ADVISOR);
    attack &= ~swapPiecesBB;
    if (makeCapture(attack, value))
        return value;

    Bitboard occupancy = position_.AllPieces();
    occupancy ^= swapPiecesBB;

    attack = AttackBB<PieceType::BISHOP>(to, occupancy);
    attack &= position_.Pieces(c, PieceType::BISHOP);
    attack &= ~swapPiecesBB;
    if (makeCapture(attack, value))
        return value;

    attack = AttackBB<PieceType::KNIGHT_TO>(to, occupancy);
    attack &= position_.Pieces(c, PieceType::KNIGHT);
    attack &= ~swapPiecesBB;
    if (makeCapture(attack, value))
        return value;

    attack = AttackBB<PieceType::CANNON>(to, occupancy);
    attack &= position_.Pieces(c, PieceType::CANNON);
    attack &= ~swapPiecesBB;
    if (makeCapture(attack, value))
        return value;
    
    attack = AttackBB<PieceType::ROOK>(to, occupancy);
    attack &= position_.Pieces(c, PieceType::ROOK);
    attack &= ~swapPiecesBB;
    if (makeCapture(attack, value))
        return value;

    attack = AttackBB<PieceType::KING>(to);
    attack &= position_.Pieces(c, PieceType::KING);
    attack &= ~swapPiecesBB;
    if (makeCapture(attack, value))
        return value;

    return 0;
}

template <Color c>
Value MovePicker::SEECapture(Move captureMove)
{
    auto from = captureMove.MoveFrom();
    auto to = captureMove.MoveTo();
    Bitboard swapPiecesBB = SquareBB(from);
    Piece attacker = position_.piece_from_square(from);
    Piece victim = position_.piece_from_square(to);
    // attacker become victim in next SEE
    Value value = GetPieceValue(TypeOfPiece(victim)) - SEE<!c>(attacker, to, swapPiecesBB);
    return value;
}

#endif