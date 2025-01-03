#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <random>

#include "../src/position.h"
#include "../src/search.h"
#include "../src/move_generator.h"
#include "../src/evaluate.h"
#include "../src/magic.h"
#include "magic_validator.h"

constexpr int kSearchDepth = 4;

#include "../src/book.h"
void Test(int searchDepth = kSearchDepth)
{
    Position position;

    // position.SetPosition("1nbakabn1/9/4c4/p1p1p1p1p/9/9/P1P1P1P1P/9/r8/RNBAKABN1 w - - 0 1");
    // position.SetPosition("1rbakabnr/9/2n1c4/p1p1p1p1p/9/6P2/P1P1P3P/1C2C1N2/9/1RBAKABR1 b - - 0 1");
    // position.SetPosition("rnbakabnr/9/2c6/p1p1C1p1p/9/1C7/P1P1P1P1P/6N2/9/1RBAKAB1R w - - 0 1");
    // position.SetPosition("rnbakabnr/9/9/p1p1C1p1p/9/1C7/P1P1P1P1P/6N2/8R/1RBAKAB2 b - - 0 1");
    // position.SetPosition("r1ba1abnr/3k5/6c2/6p1p/p3C4/3RC4/P3P1P1P/9/9/1RBAKABN1 b - - 0 1");
    // position.SetPosition("r1ba1abnr/3k5/6c2/6p1p/p3C4/3RC4/P3P1P1P/9/9/1RBAKABN1 b - - 0 1 moves g7d7");
    // position.SetPosition("rCbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/7C1/9/RNBAKABNR b - - 0 1");
    // position.SetPosition("rnbakabnr/9/7c1/p1p1C1p1p/9/9/P1P1P1P1P/1C4N2/9/1RBAKAB1R b - - 0 1 moves a6a5 b2b4");
    // position.SetPosition("r1ba1abnr/3k5/6c2/p3C1p1p/3RC4/9/P3P1P1P/6N2/9/1RBAKAB2 b - - 0 1 moves g7d7 e6d6 d8e8 e5e4");
    // position.SetPosition("r1b1kabnr/4a4/2C6/4C1pRp/p8/9/P1P1PnP1P/6n2/9/1RBAKAB2 b - - 0 1 moves e8f7 h6h5 a9a7 c7c4 g6g5 b0b1 f3e1 c4e4 e9d9 b1d1");
    // position.SetPosition("2bakab1r/9/r4a3/4p3p/p5pR1/9/P1P1PnP1P/3A1p3/3Rp4/2B1K1n2 w - - 0 1");

    position.DisplayBoard();
    // std::cout << position.GenerateFen() << std::endl;
    Search search(position);

    clock_t beg, end;
    beg = clock();
    search.IterativeDeepeningLoop(searchDepth);
    end = clock();
    auto duration = (double)(end - beg) / CLOCKS_PER_SEC;
    std::cout << "timecost:" << duration << std::endl;
    std::cout << "bestmove:" << Move2String(search.best_move()) << std::endl;
    std::cout << "score:" << search.best_score() << std::endl;
    std::cout << "searched nodes count:" << Search::search_nodes << std::endl;
}

void PlayWithHuman(int searchDepth = kSearchDepth)
{
    Position position;
    std::string fen = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1";
    // used to revert to last position
    std::stack<std::string> lastFens;
    UndoInfo undoInfo;
    while (true)
    {
        position.SetPosition(fen);
        position.DisplayBoard();
        std::cout << fen << std::endl;
        lastFens.push(fen);

        // user play first
        {
            // Check if win
            MoveGenerator moveGenerator(position);
            if (moveGenerator.CheckMoves().empty()
                && moveGenerator.CaptureMoves().empty()
                && moveGenerator.NonCaptureMoves().empty())
            {
                std::cout << "robot win" << std::endl;
                break;
            }
        }
        std::cout << "input the move:" << std::endl;
        std::string smove;
        std::cin >> smove;
        if (smove == "last")
        {
            lastFens.pop();
            if (lastFens.empty())
                fen = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1";
            else
                fen = lastFens.top();
            continue;
        }
        else if (smove == "quit")
        {
            break;
        }
        auto move = String2Move(smove);
        if (position.piece_from_square(MoveTo(move)) != Piece::NO_PIECE)
        {
            position.MakeMove(move, undoInfo);
            fen = position.GenerateFen();
        }
        else
        {
            position.MakeMove(move, undoInfo);
            if (fen.substr(fen.size() - 3) == "0 1")
                fen += " moves";
            fen += " ";
            fen += smove;
        }
        position.DisplayBoard();
        std::cout << fen << std::endl;

        // robot play
        position.SetPosition(fen);
        {
            // Check if win
            MoveGenerator moveGenerator(position);
            if (moveGenerator.CheckMoves().empty()
                && moveGenerator.CaptureMoves().empty()
                && moveGenerator.NonCaptureMoves().empty())
            {
                std::cout << "user win" << std::endl;
                break;
            }
        }
        Search search(position);
        clock_t beg, end;
        beg = clock();
        search.IterativeDeepeningLoop(searchDepth);
        end = clock();
        std::cout << "best move:" << Move2String(search.best_move()) << " score:" << search.best_score() << std::endl;
        auto duration = (double)(end - beg) / CLOCKS_PER_SEC;
        std::cout << "time cost:" << duration << std::endl;
        if (position.piece_from_square(MoveTo(search.best_move())) != Piece::NO_PIECE)
        {
            position.MakeMove(search.best_move(), undoInfo);
            fen = position.GenerateFen();
        }
        else
        {
            position.MakeMove(search.best_move(), undoInfo);
            if (fen.substr(fen.size() - 3) == "0 1")
                fen += " moves";
            fen += " ";
            fen += Move2String(search.best_move());
        }
    }
}

class RobotBattleRecorder
{
public:
    RobotBattleRecorder() = default;

    bool Open(const std::string& fileName, bool timestampSuffix = false, std::ios_base::openmode mode = std::ios_base::out)
    {
        std::string openFileName;
        if (timestampSuffix)
        {
            auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
            auto epoch = now.time_since_epoch();
            auto duration = epoch.count();
            auto dotPos = fileName.find_last_of('.');
            if (dotPos != std::string::npos)
                openFileName = fileName.substr(0, dotPos) + "_" + std::to_string(duration) + fileName.substr(dotPos);
            else
                openFileName = fileName + "_" + std::to_string(duration);
            std::cout << openFileName << std::endl;
        }
        fout_.open(openFileName, mode);
        return file_output_ = fout_.is_open();
    }

    RobotBattleRecorder& operator<<(const std::string& str)
    {
        if (file_output_)
            fout_ << str;
        else
            std::cout << str;
        return *this;
    }

private:
    std::ofstream fout_;
    bool file_output_ = false;
};

void RobotBattle(bool recordFen, const std::string& fileName, int searchDepth = kSearchDepth)
{
    // if (searchDepth == 0), use random depth, it aims to create a lot of different battle files
    std::default_random_engine e{std::random_device{}()};
    auto randomDepth = [&e](int lowerbound, int upperbound) { 
        return e() % (upperbound - lowerbound + 1) + lowerbound;
    };

    RobotBattleRecorder recorder;
    if (recordFen)
    {
        bool ret = recorder.Open(fileName, true, std::ios::out);
        if (!ret)
        {
            std::cout << "Cannot open file" << std::endl;
            return;
        }
    }

    Position position;
    std::string fen = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1";
    UndoInfo undoInfo;
    clock_t beg, end;
    int maxPly = 100;
    while (maxPly-- > 0)
    {
        position.SetPosition(fen);
        position.DisplayBoard();
        recorder << fen << "\n";
        // robot_w play
        {
            // Check if win
            MoveGenerator moveGenerator(position);
            if (moveGenerator.CheckMoves().empty()
                && moveGenerator.CaptureMoves().empty()
                && moveGenerator.NonCaptureMoves().empty())
            {
                std::cout << "robot_b win" << std::endl;
                break;
            }
        }
        beg = clock();
        Search search(position);
        search.IterativeDeepeningLoop(searchDepth == 0 ? randomDepth(2, 5) : searchDepth);
        end = clock();
        std::cout << "best move:" << Move2String(search.best_move()) << " score:" << search.best_score() << std::endl;
        auto duration = (double)(end - beg) / CLOCKS_PER_SEC;
        std::cout << "time cost:" << duration << std::endl;
        if (position.piece_from_square(MoveTo(search.best_move())) != Piece::NO_PIECE)
        {
            position.MakeMove(search.best_move(), undoInfo);
            fen = position.GenerateFen();
        }
        else
        {
            position.MakeMove(search.best_move(), undoInfo);
            if (fen.substr(fen.size() - 3) == "0 1")
                fen += " moves";
            fen += " ";
            fen += Move2String(search.best_move());
        }
        position.DisplayBoard();
        recorder << fen << "\n";

        // robot_b play
        position.SetPosition(fen);
        search.SetPosition(position);
        {
            // Check if win
            MoveGenerator moveGenerator(position);
            if (moveGenerator.CheckMoves().empty()
                && moveGenerator.CaptureMoves().empty()
                && moveGenerator.NonCaptureMoves().empty())
            {
                std::cout << "robot_w win" << std::endl;
                break;
            }
        }
        beg = clock();
        search.IterativeDeepeningLoop(searchDepth == 0 ? randomDepth(2, 5) : searchDepth);
        end = clock();
        std::cout << "best move:" << Move2String(search.best_move()) << " score:" << search.best_score() << std::endl;
        duration = (double)(end - beg) / CLOCKS_PER_SEC;
        std::cout << "time cost:" << duration << std::endl;
        if (position.piece_from_square(MoveTo(search.best_move())) != Piece::NO_PIECE)
        {
            position.MakeMove(search.best_move(), undoInfo);
            fen = position.GenerateFen();
        }
        else
        {
            position.MakeMove(search.best_move(), undoInfo);
            if (fen.substr(fen.size() - 3) == "0 1")
                fen += " moves";
            fen += " ";
            fen += Move2String(search.best_move());
        }
    }
    if (maxPly <= 0)
        std::cout << "draw" << std::endl;
}

double TestSpeed(const std::string &fileName, int searchDepth = kSearchDepth, bool showIteration = true)
{
    std::ifstream fin(fileName, std::ios::in);
    if (fin.is_open())
    {
        std::string line;
        unsigned long long searchNodesSum = 0;
        auto beg = clock();
        double maxOneStepTimeConsumption = 0;
        while (std::getline(fin, line))
        {
            Position position(line);
            Search search(position);
            auto onePlyBeg = clock();
            search.IterativeDeepeningLoop(searchDepth);
            auto onePlyEnd = clock();
            auto onePlyDuration = (double)(onePlyEnd - onePlyBeg) / CLOCKS_PER_SEC;
            maxOneStepTimeConsumption = maxOneStepTimeConsumption > onePlyDuration ? maxOneStepTimeConsumption : onePlyDuration;
            searchNodesSum += Search::search_nodes;
            if (showIteration)
                std::cout << onePlyDuration << "(" << Search::search_nodes << ")" << "; ";
        }
        if (showIteration)
            std::cout << std::endl;
        auto end = clock();
        auto duration = (double)(end - beg) / CLOCKS_PER_SEC;
        std::cout << fileName << "  (depth " << searchDepth << ": timecost " << duration << "s, search nodes " << searchNodesSum << ")" << std::endl;
        // std::cout << "max one step time consumption: " << maxOneStepTimeConsumption << std::endl;
        return duration;
    }
    std::cout << "Cannot open file" << std::endl;
    return 0;
}

int main(int argc, char* argv[])
{
    AttackInitializer::InitAttack();
    // MagicInitializer::InitMagic();

    if (argc == 1)
    {
        Test();
        return 0;
    }
    
    std::string argv1, argv2;
    int searchDepth = kSearchDepth;
    if (argc >= 2)
    {
        argv1 = argv[1];
    }
    if (argc >= 3)
    {
        std::string argv2 = argv[2];
        bool isDigit = std::all_of(argv2.begin(), argv2.end(), [](char& c) { return std::isdigit(c); });
        if (!isDigit || argv2.size() >= 3)
        {
            std::cout << "Invalid argument" << std::endl;
            return 0;
        }
        else
        {
            searchDepth = std::stoi(argv2);
        }
    }

    if (argv1 == "test")
    {
        Test(searchDepth);
    }
    else if (argv1 == "play")
    {
        PlayWithHuman(searchDepth);
    }
    else if (argv1 == "battle")
    {
        RobotBattle(true, "robot_battle.txt", searchDepth);
    }
    else if (argv1 == "speed")  // (Release will be faster)
    {
        // ============================= Ryzen 7 5800X (Debug) =============================
        /*
        Wait to update
        */

        // =============================   i5-1135G7 (Debug)   ============================= 
        /*
        robot_battle.txt  (depth 4: timecost 1.629s, search nodes 314888)
        robot_battle_1729653504763.txt  (depth 4: timecost 0.13s, search nodes 30073)
        robot_battle_1729666476581.txt  (depth 4: timecost 1.559s, search nodes 387057)
        robot_battle_1729759105954.txt  (depth 4: timecost 0.28s, search nodes 59183)
        robot_battle_1730124987011.txt  (depth 4: timecost 0.707s, search nodes 148504)
        robot_battle_1730130702948.txt  (depth 4: timecost 1.305s, search nodes 262578)
        robot_battle_1730422269049.txt  (depth 4: timecost 1.291s, search nodes 323080)
        robot_battle_1732956559883.txt  (depth 4: timecost 2.372s, search nodes 538229)
        robot_battle_1733750483345.txt  (depth 4: timecost 1.598s, search nodes 380456)
        robot_battle_1733750496914.txt  (depth 4: timecost 2.087s, search nodes 516491)
        robot_battle_1733917013899.txt  (depth 4: timecost 1.24s, search nodes 296865)
        robot_battle_1733917045094.txt  (depth 4: timecost 1.477s, search nodes 307799)
        robot_battle_1733922523712.txt  (depth 4: timecost 3.533s, search nodes 763184)
        robot_battle_1733924013886.txt  (depth 4: timecost 3.825s, search nodes 715192)
        All files cost time is: 23.033s

        robot_battle.txt  (depth 6: timecost 32.987s, search nodes 5705757)
        robot_battle_1729653504763.txt  (depth 6: timecost 1.575s, search nodes 295757)
        robot_battle_1729666476581.txt  (depth 6: timecost 47.447s, search nodes 9511521)
        robot_battle_1729759105954.txt  (depth 6: timecost 6.597s, search nodes 1171513)
        robot_battle_1730124987011.txt  (depth 6: timecost 19.964s, search nodes 3737115)
        robot_battle_1730130702948.txt  (depth 6: timecost 30.029s, search nodes 5738869)
        robot_battle_1730422269049.txt  (depth 6: timecost 21.584s, search nodes 4738617)
        robot_battle_1732956559883.txt  (depth 6: timecost 48.62s, search nodes 9417157)
        robot_battle_1733750483345.txt  (depth 6: timecost 36.8s, search nodes 7369333)
        robot_battle_1733750496914.txt  (depth 6: timecost 55.781s, search nodes 11555549)
        robot_battle_1733917013899.txt  (depth 6: timecost 31.593s, search nodes 5948272)
        robot_battle_1733917045094.txt  (depth 6: timecost 39.264s, search nodes 6634236)
        robot_battle_1733922523712.txt  (depth 6: timecost 66.128s, search nodes 12466336)
        robot_battle_1733924013886.txt  (depth 6: timecost 58.202s, search nodes 11114005)
        All files cost time is: 496.571s
        */

        if (argc == 4)
        {
            std::string fileName = argv[3];
            TestSpeed(fileName, searchDepth);
        }
        else
        {
            std::vector<std::string> fileList{
                "robot_battle.txt",
                "robot_battle_1729653504763.txt",
                "robot_battle_1729666476581.txt",
                "robot_battle_1729759105954.txt",
                "robot_battle_1730124987011.txt",
                "robot_battle_1730130702948.txt",
                "robot_battle_1730422269049.txt",
                "robot_battle_1732956559883.txt",
                "robot_battle_1733750483345.txt",
                "robot_battle_1733750496914.txt",
                "robot_battle_1733917013899.txt",
                "robot_battle_1733917045094.txt",
                "robot_battle_1733922523712.txt",
                "robot_battle_1733924013886.txt"
            };
            double totalTime = 0;
            for (auto file : fileList)
            {
                totalTime += TestSpeed(file, searchDepth, false);
            }
            std::cout << "All files cost time is: " << totalTime << "s" << std::endl;
        }
    }
    else if (argv1 == "magic")
    {
        std::vector<std::string> files{
            "robot_battle.txt",
            "robot_battle_1729653504763.txt",
            "robot_battle_1729666476581.txt",
            "robot_battle_1729759105954.txt",
            "robot_battle_1730124987011.txt",
            "robot_battle_1730130702948.txt",
            "robot_battle_1730422269049.txt",
            "robot_battle_1732956559883.txt"
        };
        MagicValidator::ValidatePseudoLegalMove(files);
        MagicValidator::ValidateLegalMove(files);
    }
    else
    {
        std::cout << "Invalid argument" << std::endl;
    }
}