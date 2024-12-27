#include <iostream>
#include <sstream>
#include "position.h"
#include "search.h"
#include "magic.h"

void Engine()
{
    AttackInitializer::InitAttack();

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
                depth = "6"; // currently we fix depth to 6

                Search search(position);

                clock_t beg, end;
                beg = clock();
                search.IterativeDeepeningLoop(std::stoi(depth));
                end = clock();
                duration = (double)(end - beg) / CLOCKS_PER_SEC;

                // research with extra 2 depth if time consumption less than 0.2 second
                if (duration < 0.2)
                {
                    depth = "8";
                    beg = clock();
                    search.IterativeDeepeningLoop(std::stoi(depth));
                    end = clock();
                }

                std::cout << "time cost:" << duration << std::endl;
                std::cout << "bestmove:" << Move2String(search.best_move()) 
                          << ", score:" << search.best_score()
                          << ", depth:" << depth << std::endl;
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

