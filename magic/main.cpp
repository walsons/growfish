#include "magic_generator.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Unkonw piece type to generate" << std::endl;
        return 0;
    }
    MagicGenerator mg;
    std::string piece = argv[1];
    if (piece == "r" || piece == "R")
        mg.PrintMagicArray<PieceType::ROOK>();
    else if (piece == "c" || piece == "C")
        mg.PrintMagicArray<PieceType::CANNON>();
    else
        std::cout << "Unkonw piece type to generate" << std::endl;
    return 0;
}

// Generated magic
constexpr Bitboard RookMagic[SQ_NUM] =
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
    (Bitboard(0x1044000883ULL) << 64) | Bitboard(0x880102002638ULL),
    (Bitboard(0x4000000800821000ULL) << 64) | Bitboard(0x2400300122002024ULL),
    (Bitboard(0x20004400002003ULL) << 64) | Bitboard(0x68100002000040ULL),
    (Bitboard(0x80000080200440ULL) << 64) | Bitboard(0x30081a0900100200ULL),
    (Bitboard(0x40000400608080ULL) << 64) | Bitboard(0x2204000010019500ULL),
    (Bitboard(0x80020a000ULL) << 64) | Bitboard(0x20000c020002000ULL),
    (Bitboard(0x2000000800182001ULL) << 64) | Bitboard(0x2202000630000402ULL),
    (Bitboard(0x2589008000820811ULL) << 64) | Bitboard(0x1808024000025000ULL),
    (Bitboard(0x300000100018404ULL) << 64) | Bitboard(0x4080202000020040ULL),
    (Bitboard(0x4000000108000100ULL) << 64) | Bitboard(0x10400210400010ULL),
    (Bitboard(0xa0004422000020ULL) << 64) | Bitboard(0xa10000000040000ULL),
    (Bitboard(0x800000402001010ULL) << 64) | Bitboard(0x10800020a8800a0ULL),
    (Bitboard(0x200002030004040ULL) << 64) | Bitboard(0x20020002000826ULL),
    (Bitboard(0x4300000202001004ULL) << 64) | Bitboard(0x804000100020881ULL),
    (Bitboard(0x400110000888ULL) << 64) | Bitboard(0x1004000087440000ULL),
    (Bitboard(0x200400200ULL) << 64) | Bitboard(0x800090c204031908ULL),
    (Bitboard(0x900100002000070aULL) << 64) | Bitboard(0x806002040001400ULL),
    (Bitboard(0x802000048002046ULL) << 64) | Bitboard(0x45104008080008ULL),
    (Bitboard(0x88000328186a001ULL) << 64) | Bitboard(0x100800100030ULL),
    (Bitboard(0x806010000ULL) << 64) | Bitboard(0x1810508010088020ULL),
    (Bitboard(0x1400040010ULL) << 64) | Bitboard(0x6009408830004000ULL),
    (Bitboard(0x1000440008ULL) << 64) | Bitboard(0x2008001100000040ULL),
    (Bitboard(0x500000200100008ULL) << 64) | Bitboard(0x10202200b0050080ULL),
    (Bitboard(0x180000210100020ULL) << 64) | Bitboard(0x410122010048821ULL),
    (Bitboard(0x200009404001cULL) << 64) | Bitboard(0x1002000024040000ULL),
    (Bitboard(0x230880084ULL) << 64) | Bitboard(0x40410000008201ULL),
    (Bitboard(0x5000002404100009ULL) << 64) | Bitboard(0x400801c00000812ULL),
    (Bitboard(0x8080008030ULL) << 64) | Bitboard(0xa0400000c28000ULL),
    (Bitboard(0x80000010001a880ULL) << 64) | Bitboard(0x62000000840040ULL),
    (Bitboard(0x900000080230080ULL) << 64) | Bitboard(0x108000200002010ULL),
    (Bitboard(0x40000840010004ULL) << 64) | Bitboard(0x800900122201400ULL),
    (Bitboard(0xa8200024400200ULL) << 64) | Bitboard(0x41800002182201cULL),
    (Bitboard(0x800000201090800ULL) << 64) | Bitboard(0x1a00140100021002ULL),
    (Bitboard(0x1010000140000a00ULL) << 64) | Bitboard(0x110008000008480ULL),
    (Bitboard(0x1610000020000200ULL) << 64) | Bitboard(0x50000101000000bULL),
    (Bitboard(0x200000d800500ULL) << 64) | Bitboard(0x400902008201010ULL),
    (Bitboard(0x7000000820840041ULL) << 64) | Bitboard(0x120101404000ULL),
    (Bitboard(0x100000400022220ULL) << 64) | Bitboard(0x8000680008008000ULL),
    (Bitboard(0x8102404000d2000ULL) << 64) | Bitboard(0x40840010c000800ULL),
    (Bitboard(0x400848610808000ULL) << 64) | Bitboard(0x80400000210004cULL),
    (Bitboard(0x20160040ULL) << 64) | Bitboard(0x480808000400000ULL),
    (Bitboard(0x50000208020060ULL) << 64) | Bitboard(0xa082204020a100ULL),
    (Bitboard(0x42000024800401ULL) << 64) | Bitboard(0x2100008800008ULL),
    (Bitboard(0x200a0a00ULL) << 64) | Bitboard(0x3020801000048003ULL),
    (Bitboard(0xc45a000005020081ULL) << 64) | Bitboard(0x800000000000ULL),
    (Bitboard(0x609000a20ULL) << 64) | Bitboard(0x10000800001284ULL),
    (Bitboard(0x40000028008c100ULL) << 64) | Bitboard(0x4040000010110030ULL),
    (Bitboard(0x20000c00008009ULL) << 64) | Bitboard(0x80024400000200ULL),
    (Bitboard(0x400000340c02000ULL) << 64) | Bitboard(0x2010000621001140ULL),
    (Bitboard(0xa60000084808080ULL) << 64) | Bitboard(0x2200050020101002ULL),
    (Bitboard(0xc0000301041002ULL) << 64) | Bitboard(0x4200020002800002ULL),
    (Bitboard(0x200000040008800ULL) << 64) | Bitboard(0xc080004004000009ULL),
    (Bitboard(0xc11000100882a00ULL) << 64) | Bitboard(0x140008988000000ULL),
    (Bitboard(0x1000000014001200ULL) << 64) | Bitboard(0x1020000430010002ULL),
    (Bitboard(0x8000001200041001ULL) << 64) | Bitboard(0x40100a0000100001ULL),
    (Bitboard(0x80000604020000ULL) << 64) | Bitboard(0x1001020000800c18ULL),
    (Bitboard(0x10404800010a0422ULL) << 64) | Bitboard(0x314800002001404ULL),
    (Bitboard(0x818011010ULL) << 64) | Bitboard(0x48200000c21200ULL),
    (Bitboard(0xa20800008ULL) << 64) | Bitboard(0x800400010868100ULL),
    (Bitboard(0x40000010200800ULL) << 64) | Bitboard(0x11510000c0800300ULL),
    (Bitboard(0x91410220ULL) << 64) | Bitboard(0x411000008000003ULL),
    (Bitboard(0x40c7000004102022ULL) << 64) | Bitboard(0x400800020020ULL),
    (Bitboard(0x1400000020020400ULL) << 64) | Bitboard(0x60180080828480ULL),
    (Bitboard(0x8800000084140080ULL) << 64) | Bitboard(0x2208488000000200ULL),
    (Bitboard(0x800000800212080ULL) << 64) | Bitboard(0x30001080100018ULL),
    (Bitboard(0x800000288008142ULL) << 64) | Bitboard(0x420000400800600ULL),
    (Bitboard(0x40000100000812ULL) << 64) | Bitboard(0x410002000480022ULL),
    (Bitboard(0x400000426081820ULL) << 64) | Bitboard(0x4004040100880440ULL),
    (Bitboard(0xa228000008100010ULL) << 64) | Bitboard(0x300040408000000ULL),
    (Bitboard(0x200000110040800ULL) << 64) | Bitboard(0x1042028000014040ULL),
    (Bitboard(0x2808000020101108ULL) << 64) | Bitboard(0x4002852000011660ULL),
    (Bitboard(0x2000011040400ULL) << 64) | Bitboard(0x8024148000000008ULL),
};
