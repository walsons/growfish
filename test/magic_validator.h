#ifndef MAGIC_VALIDATOR_H
#define MAGIC_VALIDATOR_H

#include <vector>
#include <fstream>
#include <set>

#include "../src/bitboard.h"
#include "../src/position.h"
#include "../src/function.h"
#include "../src/move_generator.h"

/* This class aims to validate if the generated magic is correct.
*  It generate pseudo legal moves(rook, cannon, knight, bishop) for a specified position by move piece in
*  board, and compare with generated pseudo legal moves by bitboard.
*/ 

class MagicValidator
{
public:
    static void Validate(const std::vector<std::string> & files)
    {
        std::ifstream fin(files[0], std::ios::in);
        std::string fen;
        if (fin.is_open())
        {
            while (std::getline(fin, fen))
            {
                Position position(fen);
                // Color c = position.side_to_move();
                // Bitboard rook = position.Pieces(c, PieceType::ROOK);
                MoveGenerator moveGenerator(position);
                MoveGenerator moveGeneratorOld(position);
                // while (rook)
                // {
                //     if (c == Color::RED)
                //     {
                //         Square s = PopLSB(rook);
                //         moveGeneratorOld.RookMove<Color::RED>(s);
                //         moveGenerator.RookMove<Color::RED>(s);
                //     }
                //     else 
                //     {
                //         Square s = PopLSB(rook);
                //         moveGeneratorOld.RookMove<Color::BLACK>(s);
                //         moveGenerator.RookMove<Color::BLACK>(s);
                //     }
                // }
                std::set<short> moves;
                std::set<short> movesOld;
                for (auto move: moveGeneratorOld.pseudo_legal_capture_moves_)
                    movesOld.insert(move);
                for (auto move: moveGeneratorOld.pseudo_legal_non_capture_moves_)
                    movesOld.insert(move);
                for (auto move: moveGenerator.pseudo_legal_capture_moves_)
                    moves.insert(move);
                for (auto move: moveGenerator.pseudo_legal_non_capture_moves_)
                    moves.insert(move);
                if (movesOld != moves)
                {
                    std::cout << "wrong: " << fen << std::endl;
                    position.DisplayBoard();
                    std::cout << "correct:" << std::endl;
                    for (auto move: movesOld)
                        std::cout << Move(move) << " ";
                    std::cout << std::endl;
                    std::cout << "magic:" << std::endl;
                    for (auto move: moves)
                        std::cout << Move(move) << " ";
                    std::cout << std::endl;

                }
            }
        }
    }
};

#endif