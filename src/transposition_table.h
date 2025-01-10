#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include <mutex>

#include "structures.h"
#include "evaluate.h"

enum class ValueType : char
{
    UNKNOWN = 0b00,
    LOWER_BOUND = 0b01,
    UPPER_BOUND = 0b10,
    EXACT = 0b11
};

class TTEntry
{
public:
    TTEntry() : key(0), value(0), type(ValueType::UNKNOWN), depth(0), generation(0), move(0) {}
    TTEntry(U64 p_key, int p_value, ValueType p_type, int p_depth, int p_generation, Move p_move)
        : key(p_key), value(p_value), type(p_type), depth(p_depth), generation(p_generation), move(p_move)
    {
    }
    TTEntry(const TTEntry& ttEntry)
        : key(ttEntry.key), value(ttEntry.value), type(ttEntry.type), depth(ttEntry.depth), generation(ttEntry.generation), move(ttEntry.move)
    {
    }
    TTEntry& operator=(const TTEntry& ttEntry)
    {
        key = (ttEntry.key);
        value = (ttEntry.value);
        type = (ttEntry.type);
        depth = (ttEntry.depth);
        generation = (ttEntry.generation);
        move = (ttEntry.move);
        return *this;
    }

public:
    std::mutex mutex;

    U64 key;
    Value value;
    ValueType type;
    Depth depth;
    int generation;
    Move move;
};

class TranspositionTable
{
public:
    TranspositionTable(size_t sizeOfMB = 32);
    void NewSearch();
    void Clear();
    void Store(U64 key, int value, int depth, Move move, ValueType type);
    TTEntry operator[](U64 key);

    // EXACT is considered as both lower and upper bound
    bool IsLowerBound(const TTEntry& ttEntry) { return int(ttEntry.type) & int(ValueType::LOWER_BOUND); }
    bool IsUpperBound(const TTEntry& ttEntry) { return int(ttEntry.type) & int(ValueType::UPPER_BOUND); }

    int AdjustSetValue(int value, int ply)
    {
        if (value > MateValue - 100)
            return value + ply;
        if (value < -MateValue + 100)
            return value - ply;
        return value;
    }
    int AdjustGetValue(int value, int ply)
    {
        if (value > MateValue - 100)
            return value - ply;
        if (value < -MateValue + 100)
            return value + ply;
        return value;
    }
    bool CanUseTT(const TTEntry& ttEntry, int depth, int ply, int beta)
    {
        int v = AdjustGetValue(ttEntry.value, ply);
        return (ttEntry.depth >= depth
                || v > MateValue - 100
                || v < -MateValue + 100
               )
               && 
               ((IsLowerBound(ttEntry) && v >= beta)
                || (IsUpperBound(ttEntry) && v < beta) // I have no idea why this would cause some cases incorrect
               );
    }

private:
    size_t size_;
    // number of items with the same key in hash table
    const int same_key_item_num = 4;
    TTEntry* tt_entry_;
    int generation_;
};

#endif