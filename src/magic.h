#ifndef MAGIC_H
#define MAGIC_H

#include "bitboard.h"

constexpr Bitboard RookMagicNumber[SQ_NUM] =
{
    (Bitboard(0x80400050040005c0ULL) << 64) | Bitboard(0x1044100004000001ULL),
    (Bitboard(0x4920000800a10000ULL) << 64) | Bitboard(0xa0010200010e0a01ULL),
    (Bitboard(0x2480009210096080ULL) << 64) | Bitboard(0x40040001106000ULL),
    (Bitboard(0x80004900202400ULL) << 64) | Bitboard(0x4010001080000010ULL),
    (Bitboard(0x280020000404400ULL) << 64) | Bitboard(0x222080880fa0020ULL),
    (Bitboard(0x180020110002000ULL) << 64) | Bitboard(0x42440802000004cULL),
    (Bitboard(0x900003080128800ULL) << 64) | Bitboard(0x1000810080404420ULL),
    (Bitboard(0x4100021840042014ULL) << 64) | Bitboard(0x80290004c000ULL),
    (Bitboard(0x40001212080002ULL) << 64) | Bitboard(0x8080008609a00090ULL),
    (Bitboard(0x200020000c000021ULL) << 64) | Bitboard(0x520080720080021ULL),
    (Bitboard(0x3c3900004208012ULL) << 64) | Bitboard(0x1060008000402a00ULL),
    (Bitboard(0x802000a2000200ULL) << 64) | Bitboard(0x9010000801000230ULL),
    (Bitboard(0x40200040800200ULL) << 64) | Bitboard(0x88100000a1084080ULL),
    (Bitboard(0x510800200805800ULL) << 64) | Bitboard(0x4002020020c01040ULL),
    (Bitboard(0xd4008000409a80a0ULL) << 64) | Bitboard(0x4803001060400080ULL),
    (Bitboard(0x200600110040044ULL) << 64) | Bitboard(0xa000030042000ULL),
    (Bitboard(0x2005400010000800ULL) << 64) | Bitboard(0x408a001404801040ULL),
    (Bitboard(0x800800004008200ULL) << 64) | Bitboard(0x42008500b11260c0ULL),
    (Bitboard(0x10041000440080ULL) << 64) | Bitboard(0x8000040220200000ULL),
    (Bitboard(0x40100800006180ULL) << 64) | Bitboard(0x90000809080201ULL),
    (Bitboard(0x201001000010ULL) << 64) | Bitboard(0x9020004840000428ULL),
    (Bitboard(0x2402012000880380ULL) << 64) | Bitboard(0x2000010089308020ULL),
    (Bitboard(0x6a085000204001ULL) << 64) | Bitboard(0x44000035100005ULL),
    (Bitboard(0x12000800100002ULL) << 64) | Bitboard(0x8006000200000600ULL),
    (Bitboard(0xa020210004108020ULL) << 64) | Bitboard(0x4002004100000488ULL),
    (Bitboard(0x101048000040990ULL) << 64) | Bitboard(0x1202840001000080ULL),
    (Bitboard(0x100004000201401ULL) << 64) | Bitboard(0x41100c002c4210ULL),
    (Bitboard(0x6051221018001081ULL) << 64) | Bitboard(0x2040001019003800ULL),
    (Bitboard(0x8000680084004100ULL) << 64) | Bitboard(0x50000000000040ULL),
    (Bitboard(0xc00011008004080ULL) << 64) | Bitboard(0x10000040008106ULL),
    (Bitboard(0x8007011008008000ULL) << 64) | Bitboard(0x1000010005000444ULL),
    (Bitboard(0x4010008004000ULL) << 64) | Bitboard(0xc280010002109000ULL),
    (Bitboard(0x40c00010004008ULL) << 64) | Bitboard(0x1a44000402000022ULL),
    (Bitboard(0x100800420002020ULL) << 64) | Bitboard(0x48a024010088020ULL),
    (Bitboard(0x400010000220ULL) << 64) | Bitboard(0x8200010798000110ULL),
    (Bitboard(0x6004400520000040ULL) << 64) | Bitboard(0x810141000021c913ULL),
    (Bitboard(0x12053a0481040001ULL) << 64) | Bitboard(0x140004010200888ULL),
    (Bitboard(0x8802100200020000ULL) << 64) | Bitboard(0x102000030304c100ULL),
    (Bitboard(0x200400040028ULL) << 64) | Bitboard(0x20000620000082a0ULL),
    (Bitboard(0x4211010200040019ULL) << 64) | Bitboard(0x18000000000000ULL),
    (Bitboard(0x1000038100040010ULL) << 64) | Bitboard(0x1400000064000ULL),
    (Bitboard(0x809000500200210ULL) << 64) | Bitboard(0x20080042260040ULL),
    (Bitboard(0x500200410140004ULL) << 64) | Bitboard(0x200000600c012ULL),
    (Bitboard(0x1200002004600041ULL) << 64) | Bitboard(0x208020000105020ULL),
    (Bitboard(0x80188050100001ULL) << 64) | Bitboard(0x890101080000a04ULL),
    (Bitboard(0x88880002c200ULL) << 64) | Bitboard(0x11406100000024ULL),
    (Bitboard(0x8c0080085000300ULL) << 64) | Bitboard(0x18200000000804ULL),
    (Bitboard(0x21880001000200ULL) << 64) | Bitboard(0x1044401008080008ULL),
    (Bitboard(0x48000808800100ULL) << 64) | Bitboard(0x484000040080001ULL),
    (Bitboard(0x200040800200ULL) << 64) | Bitboard(0x800008008220416ULL),
    (Bitboard(0x21400080020400ULL) << 64) | Bitboard(0x401040008200e40ULL),
    (Bitboard(0x20222004a0002200ULL) << 64) | Bitboard(0x702000950040042ULL),
    (Bitboard(0x800040020800ULL) << 64) | Bitboard(0x84004000200260ULL),
    (Bitboard(0x101021005090800ULL) << 64) | Bitboard(0x41810200000008ULL),
    (Bitboard(0x200008008100cfULL) << 64) | Bitboard(0x40a810003000ULL),
    (Bitboard(0x100048400020001ULL) << 64) | Bitboard(0x8c01208248024ULL),
    (Bitboard(0x240c0000410001ULL) << 64) | Bitboard(0x8400400200440ULL),
    (Bitboard(0x8400110408002ULL) << 64) | Bitboard(0x6100004800020ULL),
    (Bitboard(0x81080c900180000dULL) << 64) | Bitboard(0x4000020005580ULL),
    (Bitboard(0xa10400100101002ULL) << 64) | Bitboard(0x400080200021cULL),
    (Bitboard(0x6004400040002002ULL) << 64) | Bitboard(0x408c44044400ULL),
    (Bitboard(0x698600114700005ULL) << 64) | Bitboard(0x8004a2000000ULL),
    (Bitboard(0x10810c808808414ULL) << 64) | Bitboard(0x2000801800100ULL),
    (Bitboard(0xc0085228020008ULL) << 64) | Bitboard(0x1080000003000000ULL),
    (Bitboard(0x40091400008a0880ULL) << 64) | Bitboard(0x40000000000c20ULL),
    (Bitboard(0x8000420400020ULL) << 64) | Bitboard(0x20008040000000ULL),
    (Bitboard(0x401840000801400ULL) << 64) | Bitboard(0x4100020230440109ULL),
    (Bitboard(0x20201100001010ULL) << 64) | Bitboard(0x80008080000002ULL),
    (Bitboard(0x200140012000ULL) << 64) | Bitboard(0x280004000080200ULL),
    (Bitboard(0x4040014000022cULL) << 64) | Bitboard(0x900020d00000080ULL),
    (Bitboard(0xa218400020000101ULL) << 64) | Bitboard(0x4100010420041000ULL),
    (Bitboard(0x2000002004041044ULL) << 64) | Bitboard(0x420003000c010000ULL),
    (Bitboard(0x60000a204100ULL) << 64) | Bitboard(0x10400002001001ULL),
    (Bitboard(0x2000840108080780ULL) << 64) | Bitboard(0x8000800080000082ULL),
    (Bitboard(0x8411010400208000ULL) << 64) | Bitboard(0x1080400000220020ULL),
    (Bitboard(0x4c0000a10000a203ULL) << 64) | Bitboard(0x6000400040200023ULL),
    (Bitboard(0x1800282021320ULL) << 64) | Bitboard(0x2049000100804000ULL),
    (Bitboard(0x10c00104400108ULL) << 64) | Bitboard(0x6e00800090280100ULL),
    (Bitboard(0x201200020008408ULL) << 64) | Bitboard(0x124000800100b2ULL),
    (Bitboard(0x1880004009800110ULL) << 64) | Bitboard(0x112000022004000ULL),
    (Bitboard(0xa802000100022c0ULL) << 64) | Bitboard(0x40404000083091a2ULL),
    (Bitboard(0x440000284000521ULL) << 64) | Bitboard(0xa02000b000200010ULL),
    (Bitboard(0x4100100010408000ULL) << 64) | Bitboard(0x4000245000800011ULL),
    (Bitboard(0xc400020800480080ULL) << 64) | Bitboard(0x110414000000000ULL),
    (Bitboard(0x424c8081000004ULL) << 64) | Bitboard(0x9000402000100000ULL),
    (Bitboard(0x48983100002000ULL) << 64) | Bitboard(0x480016000080180ULL),
    (Bitboard(0x84002008a0400002ULL) << 64) | Bitboard(0x40004a000008008ULL),
    (Bitboard(0x2000c0004184ULL) << 64) | Bitboard(0x802000000208ULL),
    (Bitboard(0x806044080014001ULL) << 64) | Bitboard(0x441850000800020ULL),
    (Bitboard(0x24b0880a10040102ULL) << 64) | Bitboard(0x42ba018000000b00ULL),
};

constexpr Bitboard CannonMagicNumber[SQ_NUM] =
{
    (Bitboard(0x80400050040005c0ULL) << 64) | Bitboard(0x1044100004000001ULL),
    (Bitboard(0x4920000800a10000ULL) << 64) | Bitboard(0xa0010200010e0a01ULL),
    (Bitboard(0x2480009210096080ULL) << 64) | Bitboard(0x40040001106000ULL),
    (Bitboard(0x80004900202400ULL) << 64) | Bitboard(0x4010001080000010ULL),
    (Bitboard(0x280020000404400ULL) << 64) | Bitboard(0x222080880fa0020ULL),
    (Bitboard(0x180020110002000ULL) << 64) | Bitboard(0x42440802000004cULL),
    (Bitboard(0x900003080128800ULL) << 64) | Bitboard(0x1000810080404420ULL),
    (Bitboard(0x4100021840042014ULL) << 64) | Bitboard(0x80290004c000ULL),
    (Bitboard(0x40001212080002ULL) << 64) | Bitboard(0x8080008609a00090ULL),
    (Bitboard(0x200020000c000021ULL) << 64) | Bitboard(0x520080720080021ULL),
    (Bitboard(0x3c3900004208012ULL) << 64) | Bitboard(0x1060008000402a00ULL),
    (Bitboard(0x802000a2000200ULL) << 64) | Bitboard(0x9010000801000230ULL),
    (Bitboard(0x40200040800200ULL) << 64) | Bitboard(0x88100000a1084080ULL),
    (Bitboard(0x510800200805800ULL) << 64) | Bitboard(0x4002020020c01040ULL),
    (Bitboard(0xd4008000409a80a0ULL) << 64) | Bitboard(0x4803001060400080ULL),
    (Bitboard(0x200600110040044ULL) << 64) | Bitboard(0xa000030042000ULL),
    (Bitboard(0x2005400010000800ULL) << 64) | Bitboard(0x408a001404801040ULL),
    (Bitboard(0x800800004008200ULL) << 64) | Bitboard(0x42008500b11260c0ULL),
    (Bitboard(0x10041000440080ULL) << 64) | Bitboard(0x8000040220200000ULL),
    (Bitboard(0x40100800006180ULL) << 64) | Bitboard(0x90000809080201ULL),
    (Bitboard(0x201001000010ULL) << 64) | Bitboard(0x9020004840000428ULL),
    (Bitboard(0x2402012000880380ULL) << 64) | Bitboard(0x2000010089308020ULL),
    (Bitboard(0x6a085000204001ULL) << 64) | Bitboard(0x44000035100005ULL),
    (Bitboard(0x12000800100002ULL) << 64) | Bitboard(0x8006000200000600ULL),
    (Bitboard(0xa020210004108020ULL) << 64) | Bitboard(0x4002004100000488ULL),
    (Bitboard(0x101048000040990ULL) << 64) | Bitboard(0x1202840001000080ULL),
    (Bitboard(0x100004000201401ULL) << 64) | Bitboard(0x41100c002c4210ULL),
    (Bitboard(0x6051221018001081ULL) << 64) | Bitboard(0x2040001019003800ULL),
    (Bitboard(0x8000680084004100ULL) << 64) | Bitboard(0x50000000000040ULL),
    (Bitboard(0xc00011008004080ULL) << 64) | Bitboard(0x10000040008106ULL),
    (Bitboard(0x8007011008008000ULL) << 64) | Bitboard(0x1000010005000444ULL),
    (Bitboard(0x4010008004000ULL) << 64) | Bitboard(0xc280010002109000ULL),
    (Bitboard(0x40c00010004008ULL) << 64) | Bitboard(0x1a44000402000022ULL),
    (Bitboard(0x100800420002020ULL) << 64) | Bitboard(0x48a024010088020ULL),
    (Bitboard(0x400010000220ULL) << 64) | Bitboard(0x8200010798000110ULL),
    (Bitboard(0x6004400520000040ULL) << 64) | Bitboard(0x810141000021c913ULL),
    (Bitboard(0x12053a0481040001ULL) << 64) | Bitboard(0x140004010200888ULL),
    (Bitboard(0x8802100200020000ULL) << 64) | Bitboard(0x102000030304c100ULL),
    (Bitboard(0x200400040028ULL) << 64) | Bitboard(0x20000620000082a0ULL),
    (Bitboard(0x4211010200040019ULL) << 64) | Bitboard(0x18000000000000ULL),
    (Bitboard(0x1000038100040010ULL) << 64) | Bitboard(0x1400000064000ULL),
    (Bitboard(0x809000500200210ULL) << 64) | Bitboard(0x20080042260040ULL),
    (Bitboard(0x500200410140004ULL) << 64) | Bitboard(0x200000600c012ULL),
    (Bitboard(0x1200002004600041ULL) << 64) | Bitboard(0x208020000105020ULL),
    (Bitboard(0x80188050100001ULL) << 64) | Bitboard(0x890101080000a04ULL),
    (Bitboard(0x88880002c200ULL) << 64) | Bitboard(0x11406100000024ULL),
    (Bitboard(0x8c0080085000300ULL) << 64) | Bitboard(0x18200000000804ULL),
    (Bitboard(0x21880001000200ULL) << 64) | Bitboard(0x1044401008080008ULL),
    (Bitboard(0x48000808800100ULL) << 64) | Bitboard(0x484000040080001ULL),
    (Bitboard(0x200040800200ULL) << 64) | Bitboard(0x800008008220416ULL),
    (Bitboard(0x21400080020400ULL) << 64) | Bitboard(0x401040008200e40ULL),
    (Bitboard(0x20222004a0002200ULL) << 64) | Bitboard(0x702000950040042ULL),
    (Bitboard(0x800040020800ULL) << 64) | Bitboard(0x84004000200260ULL),
    (Bitboard(0x101021005090800ULL) << 64) | Bitboard(0x41810200000008ULL),
    (Bitboard(0x200008008100cfULL) << 64) | Bitboard(0x40a810003000ULL),
    (Bitboard(0x100048400020001ULL) << 64) | Bitboard(0x8c01208248024ULL),
    (Bitboard(0x240c0000410001ULL) << 64) | Bitboard(0x8400400200440ULL),
    (Bitboard(0x8400110408002ULL) << 64) | Bitboard(0x6100004800020ULL),
    (Bitboard(0x81080c900180000dULL) << 64) | Bitboard(0x4000020005580ULL),
    (Bitboard(0xa10400100101002ULL) << 64) | Bitboard(0x400080200021cULL),
    (Bitboard(0x6004400040002002ULL) << 64) | Bitboard(0x408c44044400ULL),
    (Bitboard(0x698600114700005ULL) << 64) | Bitboard(0x8004a2000000ULL),
    (Bitboard(0x10810c808808414ULL) << 64) | Bitboard(0x2000801800100ULL),
    (Bitboard(0xc0085228020008ULL) << 64) | Bitboard(0x1080000003000000ULL),
    (Bitboard(0x40091400008a0880ULL) << 64) | Bitboard(0x40000000000c20ULL),
    (Bitboard(0x8000420400020ULL) << 64) | Bitboard(0x20008040000000ULL),
    (Bitboard(0x401840000801400ULL) << 64) | Bitboard(0x4100020230440109ULL),
    (Bitboard(0x20201100001010ULL) << 64) | Bitboard(0x80008080000002ULL),
    (Bitboard(0x200140012000ULL) << 64) | Bitboard(0x280004000080200ULL),
    (Bitboard(0x4040014000022cULL) << 64) | Bitboard(0x900020d00000080ULL),
    (Bitboard(0xa218400020000101ULL) << 64) | Bitboard(0x4100010420041000ULL),
    (Bitboard(0x2000002004041044ULL) << 64) | Bitboard(0x420003000c010000ULL),
    (Bitboard(0x60000a204100ULL) << 64) | Bitboard(0x10400002001001ULL),
    (Bitboard(0x2000840108080780ULL) << 64) | Bitboard(0x8000800080000082ULL),
    (Bitboard(0x8411010400208000ULL) << 64) | Bitboard(0x1080400000220020ULL),
    (Bitboard(0x4c0000a10000a203ULL) << 64) | Bitboard(0x6000400040200023ULL),
    (Bitboard(0x1800282021320ULL) << 64) | Bitboard(0x2049000100804000ULL),
    (Bitboard(0x10c00104400108ULL) << 64) | Bitboard(0x6e00800090280100ULL),
    (Bitboard(0x201200020008408ULL) << 64) | Bitboard(0x124000800100b2ULL),
    (Bitboard(0x1880004009800110ULL) << 64) | Bitboard(0x112000022004000ULL),
    (Bitboard(0xa802000100022c0ULL) << 64) | Bitboard(0x40404000083091a2ULL),
    (Bitboard(0x440000284000521ULL) << 64) | Bitboard(0xa02000b000200010ULL),
    (Bitboard(0x4100100010408000ULL) << 64) | Bitboard(0x4000245000800011ULL),
    (Bitboard(0xc400020800480080ULL) << 64) | Bitboard(0x110414000000000ULL),
    (Bitboard(0x424c8081000004ULL) << 64) | Bitboard(0x9000402000100000ULL),
    (Bitboard(0x48983100002000ULL) << 64) | Bitboard(0x480016000080180ULL),
    (Bitboard(0x84002008a0400002ULL) << 64) | Bitboard(0x40004a000008008ULL),
    (Bitboard(0x2000c0004184ULL) << 64) | Bitboard(0x802000000208ULL),
    (Bitboard(0x806044080014001ULL) << 64) | Bitboard(0x441850000800020ULL),
    (Bitboard(0x24b0880a10040102ULL) << 64) | Bitboard(0x42ba018000000b00ULL),
};

constexpr Bitboard KnightMagicNumber[SQ_NUM] =
{
    (Bitboard(0x2040043028001131ULL) << 64) | Bitboard(0x1140040031ULL),
    (Bitboard(0x6018510001002200ULL) << 64) | Bitboard(0x4000040400000004ULL),
    (Bitboard(0x48040200400a2400ULL) << 64) | Bitboard(0x8010048010444810ULL),
    (Bitboard(0x804200900010408ULL) << 64) | Bitboard(0x121044000000100ULL),
    (Bitboard(0x110a4000c8000004ULL) << 64) | Bitboard(0x942000010000000ULL),
    (Bitboard(0x211000500420000ULL) << 64) | Bitboard(0x80800616002041ULL),
    (Bitboard(0x122840012080802ULL) << 64) | Bitboard(0x800403100880000ULL),
    (Bitboard(0x901800802411008ULL) << 64) | Bitboard(0x80800c001100c82ULL),
    (Bitboard(0x12820800d000008ULL) << 64) | Bitboard(0xd02002400900400ULL),
    (Bitboard(0x222900000007401ULL) << 64) | Bitboard(0x21089088882002a8ULL),
    (Bitboard(0x4810042a200080ULL) << 64) | Bitboard(0x208802020010ULL),
    (Bitboard(0x48040000310080ULL) << 64) | Bitboard(0x8080808000ULL),
    (Bitboard(0x814064000000020ULL) << 64) | Bitboard(0x4108480800001000ULL),
    (Bitboard(0x4202010009000008ULL) << 64) | Bitboard(0x12401140000408ULL),
    (Bitboard(0x4c4805000101180ULL) << 64) | Bitboard(0x1a00400000008010ULL),
    (Bitboard(0x312240e042050800ULL) << 64) | Bitboard(0x18100000000ULL),
    (Bitboard(0x14804668800061ULL) << 64) | Bitboard(0x1000201000008022ULL),
    (Bitboard(0x4000423800000100ULL) << 64) | Bitboard(0xa800004008010000ULL),
    (Bitboard(0x94304821040007ULL) << 64) | Bitboard(0x9388109408042ULL),
    (Bitboard(0x400a240808004562ULL) << 64) | Bitboard(0x5008a04062100024ULL),
    (Bitboard(0x2008048410400024ULL) << 64) | Bitboard(0x49b4200100500113ULL),
    (Bitboard(0x10804c08610a802ULL) << 64) | Bitboard(0x1010600000100100ULL),
    (Bitboard(0x1328302800a00ULL) << 64) | Bitboard(0x200060429ULL),
    (Bitboard(0x5112408841500080ULL) << 64) | Bitboard(0x8200a80084ac000ULL),
    (Bitboard(0x480c410820001301ULL) << 64) | Bitboard(0x1880080212000000ULL),
    (Bitboard(0x180814088000822ULL) << 64) | Bitboard(0x1a400041ULL),
    (Bitboard(0x2040a004100800ULL) << 64) | Bitboard(0x2000480403822800ULL),
    (Bitboard(0x120812000a01ULL) << 64) | Bitboard(0x14802002040000ULL),
    (Bitboard(0x301c50204163044ULL) << 64) | Bitboard(0x1002100000100ULL),
    (Bitboard(0x1005281320848808ULL) << 64) | Bitboard(0x40001000001000ULL),
    (Bitboard(0x8100840000ULL) << 64) | Bitboard(0x9000000021004002ULL),
    (Bitboard(0xc311004080400000ULL) << 64) | Bitboard(0x300109020428804ULL),
    (Bitboard(0xa00808240080b00ULL) << 64) | Bitboard(0x1102402002880000ULL),
    (Bitboard(0x200014222048009ULL) << 64) | Bitboard(0x2080001000300042ULL),
    (Bitboard(0x2c02150100081ULL) << 64) | Bitboard(0x460109205000020ULL),
    (Bitboard(0x20043082205441aULL) << 64) | Bitboard(0x420000101008ULL),
    (Bitboard(0x8040a040000ULL) << 64) | Bitboard(0x8000180830030408ULL),
    (Bitboard(0x6081100c080ULL) << 64) | Bitboard(0x8006251100000000ULL),
    (Bitboard(0x44000e0400884000ULL) << 64) | Bitboard(0x1000000004908082ULL),
    (Bitboard(0x840428030210200aULL) << 64) | Bitboard(0x8040008000e9000ULL),
    (Bitboard(0x10000020406000ULL) << 64) | Bitboard(0x84400000ULL),
    (Bitboard(0x1000000641210400ULL) << 64) | Bitboard(0x4801000118220049ULL),
    (Bitboard(0x4002600440882402ULL) << 64) | Bitboard(0x100020000c800000ULL),
    (Bitboard(0x881020122080c10ULL) << 64) | Bitboard(0x81002800020008ULL),
    (Bitboard(0x10050040a040448ULL) << 64) | Bitboard(0x2003002100001150ULL),
    (Bitboard(0x2220010002164300ULL) << 64) | Bitboard(0x201002080000ULL),
    (Bitboard(0x82100001019900ULL) << 64) | Bitboard(0x10000200ULL),
    (Bitboard(0x400200040804080ULL) << 64) | Bitboard(0x4100002080004b8ULL),
    (Bitboard(0xa800621220416040ULL) << 64) | Bitboard(0x12028003000180ULL),
    (Bitboard(0x80010408c8106030ULL) << 64) | Bitboard(0x100000000010405ULL),
    (Bitboard(0xd000000660300aULL) << 64) | Bitboard(0x8448020249010400ULL),
    (Bitboard(0x88004106201ULL) << 64) | Bitboard(0x402001010009494ULL),
    (Bitboard(0x800004040005180cULL) << 64) | Bitboard(0x209100134038ULL),
    (Bitboard(0x440000a00220801ULL) << 64) | Bitboard(0x10000409600000ULL),
    (Bitboard(0x400216005440a0ULL) << 64) | Bitboard(0x4000000008010081ULL),
    (Bitboard(0x4014000520460020ULL) << 64) | Bitboard(0x4088000010400290ULL),
    (Bitboard(0x140000004904ae0ULL) << 64) | Bitboard(0x4104400545002604ULL),
    (Bitboard(0x108000420181d8ULL) << 64) | Bitboard(0x9000000060482040ULL),
    (Bitboard(0x511c9020014844ULL) << 64) | Bitboard(0x800900090102045ULL),
    (Bitboard(0x4214012002080804ULL) << 64) | Bitboard(0x1801004000000212ULL),
    (Bitboard(0x4020040005001008ULL) << 64) | Bitboard(0x8502024400100000ULL),
    (Bitboard(0x26a08128141229ULL) << 64) | Bitboard(0x100080000030000ULL),
    (Bitboard(0x80c0502840401ULL) << 64) | Bitboard(0x94000408400404ULL),
    (Bitboard(0x82000200800110ULL) << 64) | Bitboard(0x82a0300100080000ULL),
    (Bitboard(0x8e0a058200a30120ULL) << 64) | Bitboard(0x6210000041800040ULL),
    (Bitboard(0x4000280000001084ULL) << 64) | Bitboard(0x1810108004410002ULL),
    (Bitboard(0x4100400438021441ULL) << 64) | Bitboard(0x844210010810031ULL),
    (Bitboard(0x8610014028000a24ULL) << 64) | Bitboard(0x2804080080012000ULL),
    (Bitboard(0x6048000200d0088ULL) << 64) | Bitboard(0x49420c000104098ULL),
    (Bitboard(0x2001000000848002ULL) << 64) | Bitboard(0x102000440230080ULL),
    (Bitboard(0x4880000000012026ULL) << 64) | Bitboard(0x101010310030080ULL),
    (Bitboard(0x1180020801000808ULL) << 64) | Bitboard(0x860040a480082010ULL),
    (Bitboard(0x800601040200808ULL) << 64) | Bitboard(0x4048806120090c00ULL),
    (Bitboard(0x3420e3900088010ULL) << 64) | Bitboard(0x80901200801c2302ULL),
    (Bitboard(0x42062260480151e0ULL) << 64) | Bitboard(0x21b0000801804080ULL),
    (Bitboard(0x980000000a0282ULL) << 64) | Bitboard(0x5008309000201042ULL),
    (Bitboard(0x4000188900040000ULL) << 64) | Bitboard(0x6a51200001000001ULL),
    (Bitboard(0xb000000600080000ULL) << 64) | Bitboard(0x40a0c08000080b0ULL),
    (Bitboard(0x100000000100000ULL) << 64) | Bitboard(0x4214440000408000ULL),
    (Bitboard(0x24001004281440ULL) << 64) | Bitboard(0x5201008001100800ULL),
    (Bitboard(0xe411008400018ULL) << 64) | Bitboard(0x300400416102000ULL),
    (Bitboard(0x1204000000000501ULL) << 64) | Bitboard(0x40200000040800ULL),
    (Bitboard(0x1600080002200000ULL) << 64) | Bitboard(0x802011c004208000ULL),
    (Bitboard(0x12000060000408ULL) << 64) | Bitboard(0x8a440468c0004ULL),
    (Bitboard(0x6040003405040220ULL) << 64) | Bitboard(0x88040400300408ULL),
    (Bitboard(0x4000884400000040ULL) << 64) | Bitboard(0x200c021100500000ULL),
    (Bitboard(0x810002c000020102ULL) << 64) | Bitboard(0xc01248038860400ULL),
    (Bitboard(0x400004250000404ULL) << 64) | Bitboard(0x4017008010812800ULL),
    (Bitboard(0x800009004014001ULL) << 64) | Bitboard(0x109810000830090ULL),
    (Bitboard(0x1008140c30068010ULL) << 64) | Bitboard(0x2110688042020020ULL),
};

constexpr Bitboard KnightToMagicNumber[SQ_NUM] =
{
    (Bitboard(0x622006000080000ULL) << 64) | Bitboard(0xe0008081402605ULL),
    (Bitboard(0x8148000800040000ULL) << 64) | Bitboard(0x10980011015c801ULL),
    (Bitboard(0x6018510001002200ULL) << 64) | Bitboard(0x4000040400000004ULL),
    (Bitboard(0x1300a1c4000000ULL) << 64) | Bitboard(0x2400044800003201ULL),
    (Bitboard(0x106003011040000ULL) << 64) | Bitboard(0x2014100000400ULL),
    (Bitboard(0x200c80850c012000ULL) << 64) | Bitboard(0x25208490cULL),
    (Bitboard(0x851800040000840ULL) << 64) | Bitboard(0x8102240001618000ULL),
    (Bitboard(0x1012100040a0820ULL) << 64) | Bitboard(0x9000000001100000ULL),
    (Bitboard(0x104e00008600800ULL) << 64) | Bitboard(0xa000087300200000ULL),
    (Bitboard(0x6a00100002400400ULL) << 64) | Bitboard(0x811000609011800ULL),
    (Bitboard(0x100138002c002000ULL) << 64) | Bitboard(0x8030000132400200ULL),
    (Bitboard(0x3040820900044200ULL) << 64) | Bitboard(0x400040000000ULL),
    (Bitboard(0x1421462800101402ULL) << 64) | Bitboard(0x250000001000048ULL),
    (Bitboard(0x400008000018400ULL) << 64) | Bitboard(0x6008000c08029801ULL),
    (Bitboard(0x5080081000018c4ULL) << 64) | Bitboard(0xa0020000400ULL),
    (Bitboard(0x5284064010200002ULL) << 64) | Bitboard(0x150208000802080ULL),
    (Bitboard(0x60804092002008a3ULL) << 64) | Bitboard(0x345000800000800ULL),
    (Bitboard(0x408a04c0c00140a0ULL) << 64) | Bitboard(0x1112002000121000ULL),
    (Bitboard(0x8024410409c00000ULL) << 64) | Bitboard(0xb628516501300081ULL),
    (Bitboard(0x322026420010800ULL) << 64) | Bitboard(0x400080052000200ULL),
    (Bitboard(0x404413612800880ULL) << 64) | Bitboard(0x810a00228104020ULL),
    (Bitboard(0x18200110040cc40ULL) << 64) | Bitboard(0x41400030100c00ULL),
    (Bitboard(0x5045040188100888ULL) << 64) | Bitboard(0x4004008c0800000ULL),
    (Bitboard(0x83100a060002040ULL) << 64) | Bitboard(0x2480001000100000ULL),
    (Bitboard(0x204085a0010002ULL) << 64) | Bitboard(0x8908200408180000ULL),
    (Bitboard(0x4004400008032008ULL) << 64) | Bitboard(0x4408484300820400ULL),
    (Bitboard(0x8080a02054000000ULL) << 64) | Bitboard(0x80c04008c001820ULL),
    (Bitboard(0x9100a4154000200ULL) << 64) | Bitboard(0x8204040402038480ULL),
    (Bitboard(0x708046086000004ULL) << 64) | Bitboard(0x40100a8080000030ULL),
    (Bitboard(0x40000812000ULL) << 64) | Bitboard(0x3010000010a8800ULL),
    (Bitboard(0x620910000800008ULL) << 64) | Bitboard(0x981000a00014042ULL),
    (Bitboard(0x10051822200000ULL) << 64) | Bitboard(0x30002004020008ULL),
    (Bitboard(0x900422350108260ULL) << 64) | Bitboard(0x240000442010ULL),
    (Bitboard(0x882008801b02009ULL) << 64) | Bitboard(0x404080080020800ULL),
    (Bitboard(0x11111008180010ULL) << 64) | Bitboard(0x100000120a01004ULL),
    (Bitboard(0x8000004002080100ULL) << 64) | Bitboard(0x902004004800100ULL),
    (Bitboard(0x500090420c20040ULL) << 64) | Bitboard(0xa0210404ULL),
    (Bitboard(0x102010c12008000ULL) << 64) | Bitboard(0x20001081004800ULL),
    (Bitboard(0x800a104018010ULL) << 64) | Bitboard(0x488102212200000ULL),
    (Bitboard(0x280500a02000ULL) << 64) | Bitboard(0x4040820b00009028ULL),
    (Bitboard(0x84cd009040002042ULL) << 64) | Bitboard(0x8280000000000110ULL),
    (Bitboard(0x200140020101002ULL) << 64) | Bitboard(0x200000911000002ULL),
    (Bitboard(0x1002400220401412ULL) << 64) | Bitboard(0x5008001140800000ULL),
    (Bitboard(0x8081820010060214ULL) << 64) | Bitboard(0x2104020000000400ULL),
    (Bitboard(0x2000020024530ULL) << 64) | Bitboard(0x500000008804800ULL),
    (Bitboard(0x2000000003000100ULL) << 64) | Bitboard(0x8001001000002080ULL),
    (Bitboard(0x2052800801010084ULL) << 64) | Bitboard(0x8000040600001008ULL),
    (Bitboard(0x4008000882848844ULL) << 64) | Bitboard(0x224a140410001004ULL),
    (Bitboard(0x22a008200046c0a0ULL) << 64) | Bitboard(0x20100004ULL),
    (Bitboard(0x606020010403408ULL) << 64) | Bitboard(0x8033082010020208ULL),
    (Bitboard(0x180400004200424ULL) << 64) | Bitboard(0x430430200001ULL),
    (Bitboard(0x401082000480004ULL) << 64) | Bitboard(0x1004021008400200ULL),
    (Bitboard(0x9801e000204004aULL) << 64) | Bitboard(0x1080200800a0100ULL),
    (Bitboard(0x230400c00088004ULL) << 64) | Bitboard(0x8000a00106a04ULL),
    (Bitboard(0x4200c22482020102ULL) << 64) | Bitboard(0x61484001086e8000ULL),
    (Bitboard(0x101101000310041ULL) << 64) | Bitboard(0xa0001004400000e0ULL),
    (Bitboard(0x800200008884ULL) << 64) | Bitboard(0x3004000046000000ULL),
    (Bitboard(0x1808041000028884ULL) << 64) | Bitboard(0x1008200000080440ULL),
    (Bitboard(0x210100c0083221ULL) << 64) | Bitboard(0xe820000083088100ULL),
    (Bitboard(0x200040100440800ULL) << 64) | Bitboard(0x1104203001420510ULL),
    (Bitboard(0xc0384000042aULL) << 64) | Bitboard(0x1200004000048004ULL),
    (Bitboard(0x10000000d0031446ULL) << 64) | Bitboard(0x1221201640040000ULL),
    (Bitboard(0x10a00000700ULL) << 64) | Bitboard(0x200204048401510ULL),
    (Bitboard(0x800100100040940ULL) << 64) | Bitboard(0x20800040822006ULL),
    (Bitboard(0x481050080800080ULL) << 64) | Bitboard(0x111020000011020ULL),
    (Bitboard(0x1022320420000440ULL) << 64) | Bitboard(0x2c9000400004000ULL),
    (Bitboard(0x44268200041802a0ULL) << 64) | Bitboard(0x286c600024000904ULL),
    (Bitboard(0x1c0a002080e0018ULL) << 64) | Bitboard(0xc94053000c01430ULL),
    (Bitboard(0x3200004014012108ULL) << 64) | Bitboard(0x401012000000000ULL),
    (Bitboard(0x8020040400001404ULL) << 64) | Bitboard(0x8428000000800e0ULL),
    (Bitboard(0xc180200210202ULL) << 64) | Bitboard(0x800400003000202ULL),
    (Bitboard(0x4180404090c20cULL) << 64) | Bitboard(0x800908088808002ULL),
    (Bitboard(0x80006900800132ULL) << 64) | Bitboard(0xa3c0100000900430ULL),
    (Bitboard(0x4040010010400040ULL) << 64) | Bitboard(0x204104020010100cULL),
    (Bitboard(0x8000000082020b00ULL) << 64) | Bitboard(0x2000040440080000ULL),
    (Bitboard(0x4001400008000008ULL) << 64) | Bitboard(0x920140c8c000842ULL),
    (Bitboard(0x1040080004200084ULL) << 64) | Bitboard(0x2400020218200048ULL),
    (Bitboard(0x1080480451800ULL) << 64) | Bitboard(0x400209630000080ULL),
    (Bitboard(0x848139021ULL) << 64) | Bitboard(0x1909008070080028ULL),
    (Bitboard(0x4440000008100c20ULL) << 64) | Bitboard(0x305082201101130ULL),
    (Bitboard(0x4000c0054802000ULL) << 64) | Bitboard(0x200004000000849ULL),
    (Bitboard(0x1400800000000ULL) << 64) | Bitboard(0x8060804010300980ULL),
    (Bitboard(0x20171090b4040000ULL) << 64) | Bitboard(0x860000010160614ULL),
    (Bitboard(0x6000303005a000ULL) << 64) | Bitboard(0x104c008009000080ULL),
    (Bitboard(0x28080009001004ULL) << 64) | Bitboard(0x24030201c80000ULL),
    (Bitboard(0x400a004b0080000ULL) << 64) | Bitboard(0x1a12814230200004ULL),
    (Bitboard(0x30081308a9201104ULL) << 64) | Bitboard(0x4889001040004000ULL),
    (Bitboard(0x4000040000001008ULL) << 64) | Bitboard(0x10030016000800a0ULL),
    (Bitboard(0x8108811104080080ULL) << 64) | Bitboard(0x802408000008240ULL),
    (Bitboard(0x5800088081100380ULL) << 64) | Bitboard(0x100004020c404ULL),
};

constexpr Bitboard BishopMagicNumber[SQ_NUM] =
{
    (Bitboard(0x622006000080000ULL) << 64) | Bitboard(0xe0008081402605ULL),
    (Bitboard(0x140a0208008540a0ULL) << 64) | Bitboard(0x6000022000ULL),
    (Bitboard(0x6018510001002200ULL) << 64) | Bitboard(0x4000040400000004ULL),
    (Bitboard(0x1300a1c4000000ULL) << 64) | Bitboard(0x2400044800003201ULL),
    (Bitboard(0x106003011040000ULL) << 64) | Bitboard(0x2014100000400ULL),
    (Bitboard(0x200c80850c012000ULL) << 64) | Bitboard(0x25208490cULL),
    (Bitboard(0x851800040000840ULL) << 64) | Bitboard(0x8102240001618000ULL),
    (Bitboard(0x16080000a000800ULL) << 64) | Bitboard(0x4402402400004400ULL),
    (Bitboard(0x8200a60842122014ULL) << 64) | Bitboard(0x80088c0000482000ULL),
    (Bitboard(0x180080008008ULL) << 64) | Bitboard(0x140000a04010400ULL),
    (Bitboard(0x8100819000200600ULL) << 64) | Bitboard(0x4000308244120a1ULL),
    (Bitboard(0x10088020840900ULL) << 64) | Bitboard(0x988800000109480ULL),
    (Bitboard(0x11900100004ULL) << 64) | Bitboard(0x8001144000000182ULL),
    (Bitboard(0x2200010048000800ULL) << 64) | Bitboard(0x8008900011100ULL),
    (Bitboard(0x9000204000000000ULL) << 64) | Bitboard(0xc004400409018b00ULL),
    (Bitboard(0x4000802218400000ULL) << 64) | Bitboard(0x101500010002801ULL),
    (Bitboard(0x2408048408402080ULL) << 64) | Bitboard(0x102050a00901cULL),
    (Bitboard(0x4008006180000c04ULL) << 64) | Bitboard(0x8008440018000000ULL),
    (Bitboard(0x10000808004140ULL) << 64) | Bitboard(0x31300040811022ULL),
    (Bitboard(0x4158200400000401ULL) << 64) | Bitboard(0x1200820010020000ULL),
    (Bitboard(0x3c840a008c8044ULL) << 64) | Bitboard(0x6000400942440ULL),
    (Bitboard(0x8504000c800a1001ULL) << 64) | Bitboard(0x400190100425ULL),
    (Bitboard(0x81180081440080ULL) << 64) | Bitboard(0x1292000428a08190ULL),
    (Bitboard(0x1a81000020248021ULL) << 64) | Bitboard(0x4120140800204800ULL),
    (Bitboard(0x890400022c00812ULL) << 64) | Bitboard(0xc800804200405020ULL),
    (Bitboard(0x601040010206ULL) << 64) | Bitboard(0x900201190010100ULL),
    (Bitboard(0x220401120000408ULL) << 64) | Bitboard(0x31024004240000ULL),
    (Bitboard(0x200888000000000aULL) << 64) | Bitboard(0x5c0088000800000ULL),
    (Bitboard(0x8000181810120100ULL) << 64) | Bitboard(0x92000002008ULL),
    (Bitboard(0x9a20020401010481ULL) << 64) | Bitboard(0x20a218080102023ULL),
    (Bitboard(0x20080040000ULL) << 64) | Bitboard(0x81098804200000ULL),
    (Bitboard(0x2a90000028000ULL) << 64) | Bitboard(0xe814010200200012ULL),
    (Bitboard(0x608048160020220ULL) << 64) | Bitboard(0xa0040000004088ULL),
    (Bitboard(0x6044001000080ULL) << 64) | Bitboard(0x40000442c0000400ULL),
    (Bitboard(0x100222a04000000ULL) << 64) | Bitboard(0x10000500930110ULL),
    (Bitboard(0x418002400800010ULL) << 64) | Bitboard(0x4003400000ULL),
    (Bitboard(0x80020e00000080ULL) << 64) | Bitboard(0x1800000800a00040ULL),
    (Bitboard(0x88000002400a4000ULL) << 64) | Bitboard(0x8100000420480000ULL),
    (Bitboard(0x1005602008000ULL) << 64) | Bitboard(0x4340010812003040ULL),
    (Bitboard(0x28401304084410ULL) << 64) | Bitboard(0x500000400ULL),
    (Bitboard(0x80080248001008ULL) << 64) | Bitboard(0x802220400c002060ULL),
    (Bitboard(0xac000450c8004000ULL) << 64) | Bitboard(0x64b900104000029ULL),
    (Bitboard(0x1010181060008400ULL) << 64) | Bitboard(0x210b000061002080ULL),
    (Bitboard(0x408a04c0c00140a0ULL) << 64) | Bitboard(0x1112002000121000ULL),
    (Bitboard(0x2801080024018000ULL) << 64) | Bitboard(0x20209105024024ULL),
    (Bitboard(0x28a080002040300ULL) << 64) | Bitboard(0x400000101003300ULL),
    (Bitboard(0x8002044000424040ULL) << 64) | Bitboard(0x3804810400220ULL),
    (Bitboard(0x2341060004440120ULL) << 64) | Bitboard(0x11000008005004ULL),
    (Bitboard(0x204008045090ULL) << 64) | Bitboard(0x42000a1000ULL),
    (Bitboard(0x1020800001030ULL) << 64) | Bitboard(0x104001001040240ULL),
    (Bitboard(0x860000880060024ULL) << 64) | Bitboard(0x1440000ULL),
    (Bitboard(0xa400a10440000c52ULL) << 64) | Bitboard(0x1080b04008001048ULL),
    (Bitboard(0x68c000080004914ULL) << 64) | Bitboard(0xa0080008802008ULL),
    (Bitboard(0x10008014004214ULL) << 64) | Bitboard(0x1020080004008000ULL),
    (Bitboard(0x44110c5100410ULL) << 64) | Bitboard(0x4000800104010010ULL),
    (Bitboard(0x4980578001012000ULL) << 64) | Bitboard(0x400d800000120200ULL),
    (Bitboard(0x400008020000109ULL) << 64) | Bitboard(0x4400001000040108ULL),
    (Bitboard(0x2004000000800ULL) << 64) | Bitboard(0x6a00000290000408ULL),
    (Bitboard(0xe41011032040000ULL) << 64) | Bitboard(0x2123490d000c0604ULL),
    (Bitboard(0x1002012000016201ULL) << 64) | Bitboard(0x344200c400100410ULL),
    (Bitboard(0x8000024005804000ULL) << 64) | Bitboard(0x110400000402210ULL),
    (Bitboard(0x4014200080000000ULL) << 64) | Bitboard(0x10800201094410c0ULL),
    (Bitboard(0x4100000800201000ULL) << 64) | Bitboard(0x26200404021c08eULL),
    (Bitboard(0x140400000a100ULL) << 64) | Bitboard(0x2001200000000dULL),
    (Bitboard(0x2c0430000088421ULL) << 64) | Bitboard(0x1044000244301008ULL),
    (Bitboard(0x1c040008000040ULL) << 64) | Bitboard(0x8408910800000040ULL),
    (Bitboard(0x4521220ULL) << 64) | Bitboard(0x40c026110004c08ULL),
    (Bitboard(0x400910210104b848ULL) << 64) | Bitboard(0x4008400001000ULL),
    (Bitboard(0x19528030281028ULL) << 64) | Bitboard(0x1030088000600ULL),
    (Bitboard(0x10302100804042ULL) << 64) | Bitboard(0x1000128000008ULL),
    (Bitboard(0x1802048240c0148ULL) << 64) | Bitboard(0x1c08500040812040ULL),
    (Bitboard(0x8000000001214c0cULL) << 64) | Bitboard(0x800080400c0320a0ULL),
    (Bitboard(0x40008002012ULL) << 64) | Bitboard(0xa0408108101102c0ULL),
    (Bitboard(0x9020000030064c2ULL) << 64) | Bitboard(0x800000000a48000ULL),
    (Bitboard(0x1044002640000040ULL) << 64) | Bitboard(0x2010008540000000ULL),
    (Bitboard(0x82500600010a00ULL) << 64) | Bitboard(0x1020000440004020ULL),
    (Bitboard(0xa200240070500800ULL) << 64) | Bitboard(0x88a0840888040180ULL),
    (Bitboard(0xca0200c00490060ULL) << 64) | Bitboard(0x400000000002080ULL),
    (Bitboard(0x2040880004c050aULL) << 64) | Bitboard(0x6249000a08001020ULL),
    (Bitboard(0x200000c010400049ULL) << 64) | Bitboard(0x424400010062100ULL),
    (Bitboard(0x10000012083ULL) << 64) | Bitboard(0x2101000000208009ULL),
    (Bitboard(0xb0a0000002400040ULL) << 64) | Bitboard(0x8405000d0000801ULL),
    (Bitboard(0x4004214008200023ULL) << 64) | Bitboard(0x2221000900801140ULL),
    (Bitboard(0x4008901880100ULL) << 64) | Bitboard(0x1258200100091804ULL),
    (Bitboard(0x5060020000d80208ULL) << 64) | Bitboard(0x602c810000810000ULL),
    (Bitboard(0x424800014002ULL) << 64) | Bitboard(0x10c480000c02000ULL),
    (Bitboard(0x280090000090080aULL) << 64) | Bitboard(0x409008020200480ULL),
    (Bitboard(0x4520902000080ULL) << 64) | Bitboard(0x23200020500000ULL),
    (Bitboard(0x8000090088000282ULL) << 64) | Bitboard(0x2004a10001000ULL),
    (Bitboard(0x1001000140100ULL) << 64) | Bitboard(0x40010074120208a0ULL),
};

template <PieceType pt>
const Bitboard* MagicNumber;
template <>
inline const Bitboard* MagicNumber<PieceType::ROOK> = RookMagicNumber;
template <>
inline const Bitboard* MagicNumber<PieceType::CANNON> = CannonMagicNumber;
template <>
inline const Bitboard* MagicNumber<PieceType::KNIGHT> = KnightMagicNumber;
template <>
inline const Bitboard* MagicNumber<PieceType::KNIGHT_TO> = KnightToMagicNumber;
template <>
inline const Bitboard* MagicNumber<PieceType::BISHOP> = BishopMagicNumber;

struct Magic
{
    Magic() = default;
    ~Magic() 
    {
        if (attacks)
        {
            delete[] attacks;
            attacks = nullptr;
        }
    }
    Bitboard mask = 0;
    unsigned shift = 0;
    Bitboard magic_number = 0;
    Bitboard* attacks = nullptr;
};

// extern Magic RookMagic[SQ_NUM];
// extern Magic CannonMagic[SQ_NUM];
// extern Magic KnightMagic[SQ_NUM];
// extern Magic BishopMagic[SQ_NUM];
template <PieceType pt>
Magic PieceMagic[SQ_NUM];
template <>
inline Magic PieceMagic<PieceType::ROOK>[SQ_NUM];
template <>
inline Magic PieceMagic<PieceType::CANNON>[SQ_NUM];
template <>
inline Magic PieceMagic<PieceType::KNIGHT>[SQ_NUM];
template <>
inline Magic PieceMagic<PieceType::KNIGHT_TO>[SQ_NUM];
template <>
inline Magic PieceMagic<PieceType::BISHOP>[SQ_NUM];

class MagicInitializer
{
public:
    static void InitMagic()
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

private:
    template <PieceType pt>
    Bitboard get_mask(Square s) const
    {
        // Only works for rook and cannon, other piece need a specialization
        static_assert(pt == PieceType::ROOK || pt == PieceType::CANNON, "Unsupported piece type");

        Bitboard mask = 0;
        int rk = RankOf(s), fl = FileOf(s);
        for (int r = rk + 1; r < RANK_NB - 1; r++) {
            mask |= Bitboard(1) << (r * FILE_NB + fl);
        }
        for (int r = rk - 1; r > RANK_0; r--) {
            mask |= Bitboard(1) << (r * FILE_NB + fl);
        }
        for (int f = fl + 1; f < FILE_NB - 1; f++) {
            mask |= Bitboard(1) << (rk * FILE_NB + f);
        }
        for (int f = fl - 1; f > FILE_A; f--) {
            mask |= Bitboard(1) << (rk * FILE_NB + f);
        }
        return mask;
    }

    template <PieceType pt>
    Bitboard attack(Square s, Bitboard occupies) const;

    template <PieceType pt>
    void init_magic(Square s)
    {
        static_assert(pt == PieceType::ROOK || pt == PieceType::CANNON 
                   || pt == PieceType::KNIGHT || pt == PieceType::KNIGHT_TO
                   || pt == PieceType::BISHOP,
                      "Unknown piece type");
        Bitboard mask = get_mask<pt>(s);
        size_t ones = count_1s(mask);
        Bitboard* indexs = new Bitboard[(1 << ones)];
        Bitboard* attacks = new Bitboard[(1 << ones)];
        for (size_t i = 0; i < (1ULL << ones); ++i)
        {
            indexs[i] = index_to_bitboard(i, ones, mask);
            attacks[i] = attack<pt>(s, indexs[i]);
        }
        auto &m = PieceMagic<pt>[s];
        m.mask = mask;
        m.shift = 128 - ones;
        m.magic_number = MagicNumber<pt>[s];
        m.attacks = new Bitboard[1 << ones];
        for (int i = 0; i < (1 << ones); ++i)
        {
            int j = transform(indexs[i], m.magic_number, m.shift);
            m.attacks[j] = attacks[i];
        }
    }

    size_t count_1s(Bitboard b) const;
    Bitboard index_to_bitboard(size_t index, size_t bits, Bitboard mask) const;
    Bitboard transform(Bitboard b, Bitboard magic, unsigned shift) const { return (b * magic) >> shift; }
};

template <>
inline Bitboard MagicInitializer::get_mask<PieceType::KNIGHT>(Square s) const
{
    Bitboard mask = 0;
    int rk = RankOf(s), fl = FileOf(s);
    Bitboard sBB = Bitboard(1) << s;
    Bitboard border = FileABB | FileIBB | RANK_0 | RANK_9;
    if (sBB & border)
    {
        int north = rk + 1, south = rk - 1, east = fl + 1, west = fl - 1;
        if (north < RANK_NB)
            mask |= Bitboard(1) << (north * FILE_NB + fl);
        if (south >= RANK_0)
            mask |= Bitboard(1) << (south * FILE_NB + fl);
        if (east < FILE_NB)
            mask |= Bitboard(1) << (rk * FILE_NB + east);
        if (west >= FILE_A)
            mask |= Bitboard(1) << (rk * FILE_NB + west);
    }
    else
    {
        int north = rk + 1, south = rk - 1, east = fl + 1, west = fl - 1;
        if (north < RANK_NB - 1)
            mask |= Bitboard(1) << (north * FILE_NB + fl);
        if (south > RANK_0)
            mask |= Bitboard(1) << (south * FILE_NB + fl);
        if (east < FILE_NB - 1)
            mask |= Bitboard(1) << (rk * FILE_NB + east);
        if (west > FILE_A)
            mask |= Bitboard(1) << (rk * FILE_NB + west);
    }
    return mask;
}

template <>
inline Bitboard MagicInitializer::get_mask<PieceType::KNIGHT_TO>(Square s) const
{
    Bitboard mask = 0;
    int rk = RankOf(s), fl = FileOf(s);
    if (rk + 1 < RANK_NB && fl + 1 < FILE_NB)
        mask |= Bitboard(1) << ((rk + 1) * FILE_NB + fl + 1);
    if (rk + 1 < RANK_NB && fl - 1 >= FILE_A)
        mask |= Bitboard(1) << ((rk + 1) * FILE_NB + fl - 1);
    if (rk - 1 >= RANK_0 && fl + 1 < FILE_NB)
        mask |= Bitboard(1) << ((rk - 1) * FILE_NB + fl + 1);
    if (rk - 1 >= RANK_0 && fl - 1 >= FILE_A)
        mask |= Bitboard(1) << ((rk - 1) * FILE_NB + fl - 1);
    return mask;
}

template <>
inline Bitboard MagicInitializer::get_mask<PieceType::BISHOP>(Square s) const
{
    Bitboard mask = 0;
    constexpr int blocks[4] = {SQ_NORTH + SQ_EAST, SQ_EAST + SQ_SOUTH, SQ_SOUTH + SQ_WEST, SQ_WEST + SQ_NORTH};
    for (auto block : blocks)
    {
        if (Distance(s, static_cast<Square>(s + block)) <= 1 && (s + block) >= SQ_A0 && (s + block) < SQ_NUM)
        {
            if (s < SQ_A5 && (s + block) < SQ_A5)
                mask |= (Bitboard(1) << (s + block));
            else if (s >= SQ_A5 && (s + block) >= SQ_A5)
                mask |= (Bitboard(1) << (s + block));
        }
    }
    return mask;
}

template <>
inline Bitboard MagicInitializer::attack<PieceType::ROOK>(Square s, Bitboard occupies) const
{
    Bitboard result = 0;
    int rk = RankOf(s), fl = FileOf(s);
    for (int r = rk + 1; r < RANK_NB; r++) 
    {
        result |= Bitboard(1) << (r * FILE_NB + fl);
        if (occupies & (Bitboard(1) << (r * FILE_NB + fl)))
            break;
    }
    for (int r = rk - 1; r >= RANK_0; r--) 
    {
        result |= Bitboard(1) << (r * FILE_NB + fl);
        if (occupies & (Bitboard(1) << (r * FILE_NB + fl)))
            break;
    }
    for (int f = fl + 1; f < FILE_NB; f++) 
    {
        result |= Bitboard(1) << (rk * FILE_NB + f);
        if (occupies & (Bitboard(1) << (rk * FILE_NB + f)))
            break;
    }
    for (int f = fl - 1; f >= FILE_A; f--) 
    {
        result |= Bitboard(1) << (rk * FILE_NB + f);
        if (occupies & (Bitboard(1) << (rk * FILE_NB + f)))
            break;
    }
    return result;
}

template <>
inline Bitboard MagicInitializer::attack<PieceType::CANNON>(Square s, Bitboard occupies) const
{
    Bitboard result = 0;
    int rk = RankOf(s), fl = FileOf(s);
    bool bridge = false;
    for (int r = rk + 1; r < RANK_NB; r++) 
    {
        if (bridge)
            result |= Bitboard(1) << (r * FILE_NB + fl);
        if (occupies & (Bitboard(1) << (r * FILE_NB + fl)))
        {
            if (!bridge)
                bridge = true;
            else
                break;
        }
    }
    bridge = false;
    for (int r = rk - 1; r >= RANK_0; r--) 
    {
        if (bridge)
            result |= Bitboard(1) << (r * FILE_NB + fl);
        if (occupies & (Bitboard(1) << (r * FILE_NB + fl)))
        {
            if (!bridge)
                bridge = true;
            else
                break;
        }
    }
    bridge = false;
    for (int f = fl + 1; f < FILE_NB; f++) 
    {
        if (bridge)
            result |= Bitboard(1) << (rk * FILE_NB + f);
        if (occupies & (Bitboard(1) << (rk * FILE_NB + f)))
        {
            if (!bridge)
                bridge = true;
            else
                break;
        }
    }
    bridge = false;
    for (int f = fl - 1; f >= FILE_A; f--) 
    {
        if (bridge)
            result |= Bitboard(1) << (rk * FILE_NB + f);
        if (occupies & (Bitboard(1) << (rk * FILE_NB + f)))
        {
            if (!bridge)
                bridge = true;
            else
                break;
        }
    }
    return result;
}

template <>
inline Bitboard MagicInitializer::attack<PieceType::KNIGHT>(Square s, Bitboard occupies) const
{
    Bitboard result = 0;
    constexpr int forwards[8] = {2 * SQ_NORTH + SQ_WEST, 2 * SQ_NORTH + SQ_EAST, 
                                 SQ_NORTH + 2 * SQ_EAST, SQ_SOUTH + 2 * SQ_EAST, 
                                 2 * SQ_SOUTH + SQ_EAST, 2 * SQ_SOUTH + SQ_WEST, 
                                 SQ_SOUTH + 2 * SQ_WEST, SQ_NORTH + 2 * SQ_WEST}; 
    constexpr int blocks[4] = {SQ_NORTH, SQ_EAST, SQ_SOUTH, SQ_WEST};
    for (int i = 0; i < 8; ++i)
    {
        if (Distance(s, static_cast<Square>(s + forwards[i])) <= 2 && (s + forwards[i]) >= SQ_A0 && (s + forwards[i]) < SQ_NUM)
        {
            if (!(occupies & (Bitboard(1) << (s + blocks[i / 2]))))
                result |= (Bitboard(1) << (s + forwards[i]));
        }
    }
    return result;
}

template <>
inline Bitboard MagicInitializer::attack<PieceType::KNIGHT_TO>(Square s, Bitboard occupies) const
{
    Bitboard result = 0;
    constexpr int forwards[8] = {2 * SQ_NORTH + SQ_EAST, 2 * SQ_EAST + SQ_NORTH, 
                                 2 * SQ_EAST + SQ_SOUTH, 2 * SQ_SOUTH + SQ_EAST, 
                                 2 * SQ_SOUTH + SQ_WEST, 2 * SQ_WEST + SQ_SOUTH, 
                                 2 * SQ_WEST + SQ_NORTH, 2 * SQ_NORTH + SQ_WEST}; 
    constexpr int blocks[4] = {SQ_NORTH + SQ_EAST, SQ_EAST + SQ_SOUTH, SQ_SOUTH + SQ_WEST, SQ_WEST + SQ_NORTH};
    for (int i = 0; i < 8; ++i)
    {
        if (Distance(s, static_cast<Square>(s + forwards[i])) <= 2 && (s + forwards[i]) >= SQ_A0 && (s + forwards[i]) < SQ_NUM)
        {
            if (!(occupies & (Bitboard(1) << (s + blocks[i / 2]))))
                result |= (Bitboard(1) << (s + forwards[i]));
        }
    }
    return result;
}

template <>
inline Bitboard MagicInitializer::attack<PieceType::BISHOP>(Square s, Bitboard occupies) const
{
    Bitboard result = 0;
    constexpr int forwards[4] = {2 * SQ_NORTH + 2 * SQ_EAST,
                                 2 * SQ_EAST + 2 * SQ_SOUTH,
                                 2 * SQ_SOUTH + 2 * SQ_WEST,
                                 2 * SQ_WEST + 2 * SQ_NORTH};
    constexpr int blocks[4] = {SQ_NORTH + SQ_EAST, SQ_EAST + SQ_SOUTH, SQ_SOUTH + SQ_WEST, SQ_WEST + SQ_NORTH};
    for (int i = 0; i < 4; ++i)
    {
        if (Distance(s, static_cast<Square>(s + forwards[i])) <= 2 && (s + forwards[i]) >= SQ_A0 && (s + forwards[i]) < SQ_NUM)
        {
            if (s < SQ_A5 && (s + forwards[i]) < SQ_A5)
            {
                if (!(occupies & (Bitboard(1) << (s + blocks[i]))))
                    result |= (Bitboard(1) << (s + forwards[i]));
            }
            else if (s >= SQ_A5 && (s + forwards[i]) >= SQ_A5)
            {
                if (!(occupies & (Bitboard(1) << (s + blocks[i]))))
                    result |= (Bitboard(1) << (s + forwards[i]));
            }
        }
    }
    return result;
}


class AttackInitializer
{
public:
    static void InitAttack()
    {
        // Attack for rook, cannon, knight, bishop
        MagicInitializer::InitMagic();

        // Attack for pawn, advisor, king
        AttackInitializer ai;
        for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST) 
        {
            ai.init_pawn_attack<Color::RED>(s);
            ai.init_pawn_attack<Color::BLACK>(s);
            ai.init_advisor_attack(s);
            ai.init_king_attack(s);
        }
    }

private:
    template <Color c>
    void init_pawn_attack(Square s) const
    {
        static constexpr Square SQ_BEG = (c == Color::RED ? SQ_A5 : SQ_A0);
        static constexpr Square SQ_END = (c == Color::RED ? SQ_NUM : SQ_A5);
        Square to[3] = { s + SQ_WEST, s + (c  == Color::RED ? SQ_NORTH : SQ_SOUTH), s + SQ_EAST };
        Bitboard b = 0;
        for (int i = 0; i < 3; ++i)
        {
            if (i == 1)  // forward
            {
                if (to[i] >= SQ_A0 && to[i] < SQ_NUM)
                    b |= SquareBB(to[i]);
            }
            else  // west and east
            {
                if (Distance(s, to[i]) <= 1 && (to[i] >= SQ_BEG && to[i] < SQ_END))
                    b |= SquareBB(to[i]);
            }
        }
        PawnAttackBB<c>[s] = b;
    }

    void init_advisor_attack(Square s) const
    {
        Square to[4] = { s + SQ_NORTH + SQ_EAST, s + SQ_EAST + SQ_SOUTH, s + SQ_SOUTH + SQ_WEST, s + SQ_WEST + SQ_NORTH };
        Bitboard b = 0;
        for (auto t : to)
        {
            if (t >= SQ_A0 && t < SQ_NUM && (SquareBB(t) & Palace))
            {
                b |= SquareBB(t);
            }
        }
        AdvisorAttackBB[s] = b;
    }

    void init_king_attack(Square s) const
    {
        Square to[4] = { s + SQ_NORTH, s + SQ_EAST, s + SQ_SOUTH, s + SQ_WEST };
        Bitboard b = 0;
        for (auto t : to)
        {
            if (t >= SQ_A0 && t < SQ_NUM && (SquareBB(t) & Palace))
            {
                b |= SquareBB(t);
            }
        }
        KingAttackBB[s] = b;
    }

public:
    template <Color c>
    static Bitboard PawnAttackBB[SQ_NUM];
    static Bitboard AdvisorAttackBB[SQ_NUM];
    static Bitboard KingAttackBB[SQ_NUM];
};


template <PieceType pt>
Bitboard Attack(Bitboard occupies, Square s) 
{
    static_assert(pt == PieceType::ROOK || pt == PieceType::CANNON || pt == PieceType::KNIGHT || pt == PieceType::KNIGHT_TO || pt == PieceType::BISHOP,
                  "Unknown piece type");
    const Magic &magic = PieceMagic<pt>[s];
    occupies &= magic.mask;
    auto index = (occupies * magic.magic_number) >> magic.shift;
    return magic.attacks[index];
}

template <PieceType pt, Color c>
Bitboard Attack(Square s)
{
    static_assert(pt == PieceType::PAWN);
    return AttackInitializer::PawnAttackBB<c>[s];
}

template <PieceType pt>
Bitboard Attack(Square s);
template <>
inline Bitboard Attack<PieceType::ADVISOR>(Square s)
{
    return AttackInitializer::AdvisorAttackBB[s];
}
template <>
inline Bitboard Attack<PieceType::KING>(Square s)
{
    return AttackInitializer::KingAttackBB[s];
}


#endif