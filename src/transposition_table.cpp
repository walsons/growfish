#include "transposition_table.h"

#include <cstring>


TranspositionTable::TranspositionTable(size_t sizeOfMB) : generation_(0)
{
    // size is MB, it will allocate 2 ^ n like 1, 2, 4... MB memory, which smaller or equal to size.
    size_ = 1; 
    for (; size_ * sizeof(TTEntry) * same_key_item_num < sizeOfMB * 1024 * 1024; size_ <<= 1)
        ;
    size_ >>= 1;
    tt_entry_ = new TTEntry[size_ * sizeof(TTEntry) * 4];
    Clear();
}

void TranspositionTable::NewSearch() { generation_++; }

void TranspositionTable::Clear() { std::memset((void*)tt_entry_, 0, size_ * sizeof(TTEntry) * 4); }

void TranspositionTable::Store(U64 key, int value, int depth, Move move, ValueType type)
{
    int index = (key & (size_ - 1) * same_key_item_num);
    TTEntry* replace = &tt_entry_[index];
    for (int i = 0; i < same_key_item_num; i++)
    {
        if (tt_entry_[index + i].key == 0 || tt_entry_[index + i].key == key)
        {
            replace = &tt_entry_[index + i];
            break;
        }
        else
        {
            if (tt_entry_[index + i].generation < replace->generation ||
                tt_entry_[index + i].depth < replace->depth)
            {
                replace = &tt_entry_[index + i];
            }
        }
    }
    std::lock_guard<std::mutex> lock(replace->mutex);
    *replace = TTEntry{key, value, type, depth, generation_, move};
}

TTEntry TranspositionTable::operator[](U64 key)
{
    TTEntry ttEntry;
    int index = (key & (size_ - 1) * same_key_item_num);
    for (int i = 0; i < same_key_item_num; i++)
    {
        if (tt_entry_[index + i].key == key)
        {
            std::lock_guard<std::mutex> lock(tt_entry_[index + i].mutex);
            ttEntry = tt_entry_[index + i];
            break;
        }
    }
    return ttEntry;
}

TranspositionTable TT;
