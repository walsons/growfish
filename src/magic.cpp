#include "magic.h"


void MagicInitializer::InitMagic()
{
    MagicInitializer mi;
    for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST) 
    {
        mi.init_magic<PieceType::ROOK>(s);
        mi.init_magic<PieceType::CANNON>(s);
        mi.init_magic<PieceType::KNIGHT>(s);
        mi.init_magic<PieceType::KNIGHT_TO>(s);
        mi.init_magic<PieceType::BISHOP>(s);
    }
}

Bitboard AttackInitializer::PawnAttackBB[NUM(Color::COLOR_NUM)][SQ_NUM] = {};
Bitboard AttackInitializer::PawnToAttackBB[NUM(Color::COLOR_NUM)][SQ_NUM] = {};
Bitboard AttackInitializer::AdvisorAttackBB[SQ_NUM] = {};
Bitboard AttackInitializer::KingAttackBB[SQ_NUM] = {};
