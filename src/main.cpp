#include <iostream>
#include <sstream>
#include "position.h"
#include "search.h"
#include "magic.h"

void Engine()
{
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

                // currently we use default value for SearchCondition
                SearchCondition sc;
                Search search(position);
                clock_t beg, end;
                beg = clock();
                search.Start(sc);
                end = clock();
                duration = (double)(end - beg) / CLOCKS_PER_SEC;
                std::cout << "time cost (6 depth):" << duration << std::endl;

                // research with extra 2 depth if time consumption less than 0.4 second
                // and set time cost limit to 10s
                if (duration < 0.4)
                {
                    sc.depth = 8;
                    sc.time = 10000;
                    beg = clock();
                    search.Start(sc);
                    end = clock();
                    duration = (double)(end - beg) / CLOCKS_PER_SEC;
                    std::cout << "time cost (8 depth):" << duration << std::endl;
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

