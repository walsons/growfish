#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include <list>
#include <atomic>
#include <thread>

#include "position.h"

struct SearchStack
{
    std::vector<Move> pv;
    Move current_move;
    Move killer_move[2];
};

class Search
{
public:
    Search(const Position& position) : root_position_(position) {}
    void SetPosition(const Position& position) { root_position_ = position; }
    void IterativeDeepeningLoop(Depth maxDepth);

    Move best_move() { return best_move_; }
    Value best_score() { return best_score_; }

private:
    bool search_book(std::string fen);
    void generate_root_moves();
    void root_search(Depth depth, SearchStack ss[], size_t threadIndex);
    void thread_root_search(Depth depth, SearchStack ss[], size_t threadIndex, Position rootPosition, std::list<Move> rootMoves);
    Value search(Position& position, Depth depth, Value alpha, Value beta, SearchStack ss[], Ply ply, size_t threadIndex);
    // Quiescence search
    Value qsearch(Position& position, Value alpha, Value beta, SearchStack ss[], Ply ply, size_t threadIndex);

public:
    // This number is used to calculate how many nodes have been searched
    static std::atomic<unsigned long long> search_nodes;

private:
    Position root_position_;
    std::list<Move> root_moves_;
    Move best_move_;
    Value best_score_;
    // Stop search, also a flag to indicate the return value is not valid
    // This is used to stop the search when the given time is up
    bool abort_search_;
    bool print_pv_move_ = false;
};

#endif