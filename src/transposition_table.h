#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include <mutex>
#include <atomic>

#include "structures.h"
#include "evaluate.h"

enum class ValueType : char
{
    UNKNOWN = 0b00,
    LOWER_BOUND = 0b01,
    UPPER_BOUND = 0b10,
    EXACT = 0b11
};

template <typename T>
union U64Bit
{
    U64 u64;
    T var;
};

template <typename T>
U64 U64BitForward(T var)
{
    U64Bit<T> bits{};
    bits.var = var;
    return bits.u64;
}
template <typename T>
T U64BitBack(U64 u64)
{
    U64Bit<T> bits{};
    bits.u64 = u64;
    return bits.var;
}

class TTEntryX
{
public:
    int generation() { return U64BitBack<int>(h_data >> generation_shift_); }
    int value() { return U64BitBack<int>(h_data >> value_shift_); }
    Move move() { return U64BitBack<Move>(h_data >> move_shift_); }
    int depth() { return U64BitBack<int>(h_data >> depth_shift_); }
    ValueType type() { return U64BitBack<ValueType>(h_data >> type_shift_); }

    U64 h_key = 0;
    U64 h_data = 0;
private:
    static constexpr int generation_shift_ = 40;
    static constexpr int value_shift_ = 24;
    static constexpr int move_shift_ = 8;
    static constexpr int depth_shift_ = 2;
    static constexpr int type_shift_ = 0;
};

class TTEntry
{
public:
    TTEntry()
    {
        h_key = 0;
        h_data = 0;
    }
    TTEntry(U64 key, U64 data)
    {
        h_key = key;
        h_data = data;
    }
    TTEntry(U64 key, int generation, int value, Move move, int depth, ValueType type)
    {
        h_key = key;
        h_data = Data(generation, value, depth, move, type);
    }

    int generation() { return U64BitBack<int>((h_data >> generation_shift_) & 0xFFFFFF); }
    int value() { return U64BitBack<short>((h_data >> value_shift_) & 0xFFFF); }
    Move move() { return U64BitBack<Move>((h_data >> move_shift_) & 0xFFFF); }
    int depth() { return U64BitBack<int>((h_data >> depth_shift_) & 0b111111); }
    ValueType type() { return U64BitBack<ValueType>((h_data >> type_shift_) & 0b11); }

    static U64 Data(int generation, int value, int depth, Move move, ValueType type)
    {
        short s_value = static_cast<short>(value);
        U64 data = ((U64BitForward(generation) & 0xFFFFFF) << generation_shift_)
                 | ((U64BitForward(s_value) & 0xFFFF) << value_shift_)
                 | ((U64BitForward(move) & 0xFFFF) << move_shift_)
                 | ((U64BitForward(depth) & 0b111111) << depth_shift_)
                 | ((U64BitForward(type) & 0b11) << type_shift_);
        return data;
    }

private:
    static constexpr int generation_shift_ = 40;
    static constexpr int value_shift_ = 24;
    static constexpr int move_shift_ = 8;
    static constexpr int depth_shift_ = 2;
    static constexpr int type_shift_ = 0;

public:
    std::mutex mutex;

    std::atomic<U64> h_key;
    std::atomic<U64> h_data;

/*
    U64 key;         // 8 bytes
    int generation;  // 3 bytes
    int value;       // 2 bytes
    Move move;       // 2 bytes
    int depth;       // 6 bits (less than 64)
    ValueType type;  // 2 bits
*/
};

class TranspositionTable
{
public:
    TranspositionTable(size_t sizeOfMB = 32);
    void NewSearch();
    void Clear();

    void Store(U64 key, int value, int depth, Move move, ValueType type);
    TTEntryX operator[](U64 key);

    // EXACT is considered as both lower and upper bound
    bool IsLowerBound(TTEntryX& ttEntry) { return int(ttEntry.type()) & int(ValueType::LOWER_BOUND); }
    bool IsUpperBound(TTEntryX& ttEntry) { return int(ttEntry.type()) & int(ValueType::UPPER_BOUND); }

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
    bool CanUseTT(TTEntryX& ttEntry, int depth, int ply, int beta)
    {
        int v = AdjustGetValue(ttEntry.value(), ply);
        return (ttEntry.depth() >= depth
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