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
const int MVV_LVA[14][14] = {
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
    int score;
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
        QSEARCH_CAPTURE,
        END,
    };

    MovePicker(const Position& position, Move ttMove, Move killerMove[], Phase phase = Phase::TT);
    Move NextMove();

private:
    std::list<ScoreMove> GenerateCheckMove();
    std::list<ScoreMove> GenerateCaptureMove();
    std::list<ScoreMove> GenerateCaptureMove_only_mvv_lva();
    std::list<ScoreMove> GenerateNonCaptureMove();

    template <Color c>
    int SEE(Square from, Square to, Position &position, Bitboard &swapPiecesBB)
    {
        Piece p = position.piece_from_square(from);

        bool stop = false;
        auto makeCapture = [this, &stop, &position, &swapPiecesBB, to, p](Bitboard attack) {
            if (attack)
            {
                stop = true;
                Square from = PopLSB(attack);
                swapPiecesBB |= SquareBB(from);
                int value = GetPieceValue(TypeOfPiece(p)) - SEE<!c>(from, to, position, swapPiecesBB);
                return value > 0 ? value : 0;
            }
            return 0;
        };

        Bitboard pAttack = Attack<PieceType::PAWN_TO, c>(to);
        pAttack &= position.Pieces(c, PieceType::PAWN);
        pAttack &= ~swapPiecesBB;
        int value = makeCapture(pAttack);
        if (stop)
            return value;
        
        Bitboard aAttack = Attack<PieceType::ADVISOR>(to);
        aAttack &= position.Pieces(c, PieceType::ADVISOR);
        aAttack &= ~swapPiecesBB;
        value = makeCapture(aAttack);
        if (stop)
            return value;

        Bitboard occupancy = position.AllPieces();
        occupancy ^= swapPiecesBB;

        Bitboard bAttack = Attack<PieceType::BISHOP>(occupancy, to);
        bAttack &= position.Pieces(c, PieceType::BISHOP);
        bAttack &= ~swapPiecesBB;
        value = makeCapture(aAttack);
        if (stop)
            return value;

        Bitboard nAttack = Attack<PieceType::KNIGHT_TO>(occupancy, to);
        nAttack &= position.Pieces(c, PieceType::KNIGHT);
        nAttack &= ~swapPiecesBB;
        value = makeCapture(nAttack);
        if (stop)
            return value;

        Bitboard cAttack = Attack<PieceType::CANNON>(occupancy, to);
        cAttack &= position.Pieces(c, PieceType::CANNON);
        cAttack &= ~swapPiecesBB;
        value = makeCapture(cAttack);
        if (stop)
            return value;
        
        Bitboard rAttack = Attack<PieceType::ROOK>(occupancy, to);
        rAttack &= position.Pieces(c, PieceType::ROOK);
        rAttack &= ~swapPiecesBB;
        value = makeCapture(rAttack);
        if (stop)
            return value;

        Bitboard kAttack = Attack<PieceType::KING>(to);
        kAttack &= position.Pieces(c, PieceType::KING);
        kAttack &= ~swapPiecesBB;
        value = makeCapture(kAttack);
        if (stop)
            return value;

        return 0;
    }

    template <Color c>
    int SEECapture(Move captureMove)
    {
        Bitboard swapPiecesBB = 0;
        auto from = captureMove.MoveFrom();
        auto to = captureMove.MoveTo();
        swapPiecesBB |= SquareBB(from);
        int value = GetPieceValue(TypeOfPiece(position_.piece_from_square(to))) - SEE<!c>(from, to, position_, swapPiecesBB);
        return value;
    }

private:
    Position position_;
    Move tt_move_;
    Move killer_move1_;
    Move killer_move2_;
    MoveGenerator move_generator_;
    Phase phase_;
    std::list<ScoreMove> moves_;
    std::list<ScoreMove> bad_captures_;
};

#endif