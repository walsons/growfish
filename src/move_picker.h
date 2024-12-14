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
    int SEE(Square s, Position &position)
    {
        Piece p = position.piece_from_square(s);

        bool stop = false;
        auto makeCapture = [this, &stop, &position, s, p](Bitboard attack) {
            if (attack)
            {
                stop = true;
                Square t = PopLSB(attack);
                UndoInfo undoInfo;
                Move move = ConstructMove(t, s);
                position.SimpleMakeMove(move, undoInfo);
                int value = GetPieceValue(TypeOfPiece(p)) - SEE<!c>(s, position);
                position.SimpleUndoMove(undoInfo);
                return value > 0 ? value : 0;
            }
            return 0;
        };

        Bitboard pAttack = Attack<PieceType::PAWN_TO, c>(s);
        pAttack &= position.Pieces(c, PieceType::PAWN);
        int value = makeCapture(pAttack);
        if (stop)
            return value;
        
        Bitboard aAttack = Attack<PieceType::ADVISOR>(s);
        aAttack &= position.Pieces(c, PieceType::ADVISOR);
        value = makeCapture(aAttack);
        if (stop)
            return value;

        Bitboard occupies = position.AllPieces();

        Bitboard bAttack = Attack<PieceType::BISHOP>(occupies, s);
        bAttack &= position.Pieces(c, PieceType::BISHOP);
        value = makeCapture(aAttack);
        if (stop)
            return value;

        Bitboard nAttack = Attack<PieceType::KNIGHT_TO>(occupies, s);
        nAttack &= position.Pieces(c, PieceType::KNIGHT);
        value = makeCapture(nAttack);
        if (stop)
            return value;

        Bitboard cAttack = Attack<PieceType::CANNON>(occupies, s);
        cAttack &= position.Pieces(c, PieceType::CANNON);
        value = makeCapture(cAttack);
        if (stop)
            return value;
        
        Bitboard rAttack = Attack<PieceType::ROOK>(occupies, s);
        rAttack &= position.Pieces(c, PieceType::ROOK);
        value = makeCapture(rAttack);
        if (stop)
            return value;

        Bitboard kAttack = Attack<PieceType::KING>(s);
        kAttack &= position.Pieces(c, PieceType::KING);
        value = makeCapture(kAttack);
        if (stop)
            return value;

        return 0;
    }

    template <Color c>
    int SEECapture(Move captureMove)
    {
        Position position = position_;
        Square s = captureMove.MoveTo();
        Piece p = position.piece_from_square(s);
        UndoInfo undoInfo;
        position.SimpleMakeMove(captureMove, undoInfo);
        int value = GetPieceValue(TypeOfPiece(p)) - SEE<!c>(s, position);
        position.SimpleUndoMove(undoInfo);
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