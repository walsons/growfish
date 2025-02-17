#ifndef BOOK_H
#define BOOK_H

#include "structures.h"
#include <unordered_map>

class Book
{
public:
    Book();
    Move SearchBestMove(std::string key) const;

private:
    std::unordered_map<std::string, Move> book_;
};

#endif