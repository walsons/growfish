#include "book.h"
#include "function.h"

#include <iostream>
#include <fstream>
#include <string>

Book::Book()
{
    std::ifstream file("fendata.json");
    if (!file.is_open()) {
        std::cerr << "Unable to open book!" << std::endl;
    }
    std::string line;
    // skip '{'
    while (std::getline(file, line)) {
        if (line[0] == '{')
            continue;
        if (line[0] == '}')
            break;
        auto pos = line.find(':');
        auto key = line.substr(5, pos - 6);
        auto move = line.substr(pos + 3, 4);
        book_.insert({key, String2Move(move)});
    }
    file.close();
}

Move Book::SearchBestMove(std::string key) const
{
    auto it = book_.find(key);
    return it != book_.end() ? it->second : Move(0);
}

Book BOOK;