#ifndef HISTORY_H
#define HISTORY_H

#include "function.h"
#include "position.h"

class History {
public:
    History();
    void Clear();
    void Success(const Position &position, Move move, int depth);
    Value HistoryValue(const Position &position, Move move) const;

public:
    static const Value kHistoryMax = (1 << 13);

private:
    Value history_[PieceIndex(Piece::PIECE_NUM)][Square::SQ_NUM];
};

#endif