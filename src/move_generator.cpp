#include "move_generator.h"

bool MoveGenerator::IsLegalMove(Move move)
{
    return position_.IsLegalMove(move);

    // Following code can be refered to optimize in the future
    // if (!position_.IsSelfChecked())
    // {
    //     if (from != kPos)
    //     {
    //         if (from / 9 != kRow && from % 9 != kCol && to / 9 != kRow && to % 9 != kCol && Distance(from, kPos) > 1)
    //         {
    //             return true;
    //         }
    //     }
    // }
    // else
    // {
    //     if (from != kPos)
    //     {
    //         if (from / 9 != kRow && from % 9 != kCol && to / 9 != kRow && to % 9 != kCol && Distance(to, kPos) > 2)
    //         {
    //             return false;
    //         }
    //     }
    // }
}