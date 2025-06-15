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

#ifndef UCI_H_INCLUDED
#define UCI_H_INCLUDED

#include <string>

#include "types.h"

namespace Stockfish {

class Position;

namespace UCI {

std::string square(Square s);
Square to_square(std::string s);
std::string move(Move m);
Move to_move(const std::string& str);

// It only accepts FEN strings without moves. If it encounters moves, it will ignore them, 
// use the Position::do_move method to apply the moves."
// FEN strings example: "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1"
void position(std::string fen, Position& pos);

} // namespace UCI

} // namespace Stockfish

#endif // #ifndef UCI_H_INCLUDED
