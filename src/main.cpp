#include <iostream>
#include <sstream>
#include "position.h"
#include "search.h"
#include "magic.h"

#ifndef DISABLE_PIKAFISH_NNUE_ADAPTER
#include "pikafish_nnue_adapter/bitboard.h"
#include "pikafish_nnue_adapter/position.h"
#include "pikafish_nnue_adapter/uci.h"
#endif

void Engine()
{

#ifndef DISABLE_PIKAFISH_NNUE_ADAPTER
    Stockfish::Bitboards::init();
    Stockfish::Eval::NNUE::init();
#endif

    std::cout << "***** growfish *****" << std::endl;

    double duration;
    std::string line;
    Position position;
    while (true)
    {
        bool flag = true;
        std::getline(std::cin, line);
        std::stringstream ss(line);
        std::string token;
        while (ss >> token)
        {
            if (token == "fen")
            {
                // TODO: startpos and append moves
                std::string fen;
                ss >> fen;
                auto pos = line.find(fen);
                fen = line.substr(pos);

                position.SetPosition(fen);

                flag = false;
            }
            else if (token == "setoption" || token == "ucinewgame")
            {
                flag = false;
            }
            else if (token == "isready")
            {
                std::cout << "readyok" << std::endl;
                flag = false;
            }
            else if (token == "depth")
            {
                std::string depth;
                ss >> depth;

                SearchCondition sc;
                sc.depth = stoi(depth);
                Search search(position);
                clock_t beg, end;
                beg = clock();
                search.Start(sc);
                end = clock();
                duration = (double)(end - beg) / CLOCKS_PER_SEC;
                std::cout << "time cost (" << depth << " depth):" << duration << std::endl;
                std::cout << "bestmove:" << Move2String(search.best_move()) 
                          << ", score:" << search.best_score() << std::endl;
            }
            else if (token == "best")
            {
                // Probably the best strategy for the current engine when it comes to both depth and time.
                // Firstly, use default search condition
                SearchCondition sc;
                Search search(position);
                clock_t beg, end;
                beg = clock();
                search.Start(sc);
                end = clock();
                duration = (double)(end - beg) / CLOCKS_PER_SEC;
                std::cout << "time cost (8 depth):" << duration << std::endl;

                // Second, re-search with extra 2 depth if time consumption less than 0.4s, and set time cost limit to 10s
                if (duration < 0.4)
                {
                    sc.depth = 10;
                    sc.time = 10000;
                    beg = clock();
                    search.Start(sc);
                    end = clock();
                    duration = (double)(end - beg) / CLOCKS_PER_SEC;
                    std::cout << "time cost (10 depth):" << duration << std::endl;
                }

                std::cout << "bestmove:" << Move2String(search.best_move()) 
                          << ", score:" << search.best_score() << std::endl;
            }
            else if (token == "d")
            {
                position.DisplayBoard();
            }
            else if (token == "dd")
            {
                position.DisplayBoard(true);
            }
        }
        if (flag)
            std::cout << "**********" << std::endl;
    }
}

int main()
{
    Engine();
}

