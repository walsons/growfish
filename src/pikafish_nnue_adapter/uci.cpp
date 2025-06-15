/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2022 The Stockfish developers (see AUTHORS file)

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <sstream>

#include "position.h"
#include "uci.h"

namespace Stockfish {

namespace {

    void position(Position& pos, std::istringstream& is) {

        std::string token, fen;

        while (is >> token && token != "moves")
            fen += token + " ";
        pos.set(fen);

    }

} // anonymous namespace


/// UCI::square() converts a Square to a string in algebraic notation (g1, a7, etc.)
std::string UCI::square(Square s) {
    return std::string{ char('a' + file_of(s)), char('0' + rank_of(s)) };
}

Square UCI::to_square(std::string s) {
    return Square((s[0] - 'a') + (s[1] - '0') * FILE_NB);
}

std::string UCI::move(Move m) {
    Square from = from_sq(m);
    Square to = to_sq(m);

    std::string move = UCI::square(from) + UCI::square(to);
    return move;
}

Move UCI::to_move(const std::string& str) {
    Square from = UCI::to_square(str.substr(0, 2));
    Square to = UCI::to_square(str.substr(2, 2));

    Move move = make_move(from, to);
    return move;
}

void UCI::position(std::string fen, Position& pos) {
    std::istringstream is(fen);
    Stockfish::position(pos, is);
}

} // namespace Stockfish
