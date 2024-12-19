#ifndef BOOK_H
#define BOOK_H

#include "structures.h"
#include "unordered_map"

class Book
{
public:
    Book();
    // Move SearchBestMove(unsigned long long key);
    Move SearchBestMove(std::string key);

private:
    // std::unordered_map<unsigned long long, Move> book_;
    std::unordered_map<std::string, Move> book_;
};

#endif