#include "position.h"

#include <iostream>
#include <sstream>

#include "bitboard.h"
#include "magic.h"

#ifndef DISABLE_PIKAFISH_NNUE_ADAPTER
#include "pikafish_nnue_adapter/uci.h"
#endif


void Position::MakeMove(Move move, UndoInfo& undoInfo)
{
    auto from = MoveFrom(move);
    auto to = MoveTo(move);

    key_ ^= Zobrist::PieceSquareZobrist(board_[from], from);
    key_ ^= Zobrist::PieceSquareZobrist(Piece::NO_PIECE, from);
    key_ ^= Zobrist::PieceSquareZobrist(board_[to], to);
    key_ ^= Zobrist::PieceSquareZobrist(board_[from], to);

    undoInfo = { move, board_[to] };
    move_piece(from, to);
    // board_[to] = board_[from];
    // board_[from] = Piece::NO_PIECE;

    side_to_move_ = (side_to_move_ == Color::BLACK ? Color::RED : Color::BLACK);
    key_ ^= Zobrist::BlackToMoveZobrist();

#ifndef DISABLE_PIKAFISH_NNUE_ADAPTER
    Stockfish::Move stockfishMove = static_cast<Stockfish::Move>((from << 7) | to);
    sp_->do_move(stockfishMove);
#endif
}

void Position::UndoMove(const UndoInfo& undoInfo)
{
    auto from = MoveFrom(undoInfo.move);
    auto to = MoveTo(undoInfo.move);

    // board_[from] = board_[to];
    // board_[to] = undoInfo.piece;
    move_piece(to, from);
    put_piece(undoInfo.piece, to);

    key_ ^= Zobrist::PieceSquareZobrist(board_[from], from);
    key_ ^= Zobrist::PieceSquareZobrist(Piece::NO_PIECE, from);
    key_ ^= Zobrist::PieceSquareZobrist(board_[to], to);
    key_ ^= Zobrist::PieceSquareZobrist(board_[from], to);

    key_ ^= Zobrist::BlackToMoveZobrist();
    side_to_move_ = (side_to_move_ == Color::BLACK ? Color::RED : Color::BLACK);

#ifndef DISABLE_PIKAFISH_NNUE_ADAPTER
    Stockfish::Move stockfishMove = static_cast<Stockfish::Move>((from << 7) | to);
    sp_->undo_move(stockfishMove);
#endif
}

void Position::MakeNULLMove()
{
    side_to_move_ = (side_to_move_ == Color::BLACK ? Color::RED : Color::BLACK);
    key_ ^= Zobrist::BlackToMoveZobrist();

#ifndef DISABLE_PIKAFISH_NNUE_ADAPTER
    sp_->do_null_move();
#endif
}

void Position::UndoNULLMove()
{
    side_to_move_ = (side_to_move_ == Color::BLACK ? Color::RED : Color::BLACK);
    key_ ^= Zobrist::BlackToMoveZobrist();

#ifndef DISABLE_PIKAFISH_NNUE_ADAPTER
    sp_->undo_null_move();
#endif
}

void Position::SetPosition(const std::string& fen)
{

#ifndef DISABLE_PIKAFISH_NNUE_ADAPTER
    sp_ = std::make_shared<Stockfish::Position>();
    Stockfish::UCI::position(fen, *sp_);
#endif

    std::memset(by_type_bb_, 0, sizeof(Bitboard) * NUM(PieceType::PIECE_TYPE_NUM));
    std::memset(by_color_bb_, 0, sizeof(Bitboard) * NUM(Color::COLOR_NUM));
    std::memset(piece_count_, 0, sizeof(int) * NUM(Piece::PIECE_NUM));
    key_ = 0;

    std::istringstream ss(fen);

    std::string fenPos;
    ss >> fenPos;
    std::string boardStr;
    std::string segStr;
    for (auto c : fenPos)
    {
        if (c == ' ')
            break;
        else if (c > '0' && c <= '9')
            segStr += std::string((c - '0'), '0');
        else if (c == '/')
        {
            boardStr = segStr + boardStr;
            segStr.clear();
            continue;
        }
        else
            segStr += c;
    }
    boardStr = segStr + boardStr;
    for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST)
    {
        board_[s] = PieceFromChar(boardStr[s]);
        by_type_bb_[NUM(TypeOfPiece(board_[s]))] |= SquareBB(s);
        if (board_[s] != Piece::NO_PIECE)
            by_color_bb_[NUM(ColorOfPiece(board_[s]))] |= SquareBB(s);
    }
    for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST)
    {
        key_ ^= Zobrist::PieceSquareZobrist(board_[s], s);
    }

    std::string fenSideToMove;
    ss >> fenSideToMove;
    side_to_move_ = (fenSideToMove == "w" ? Color::RED : Color::BLACK);
    if (side_to_move_ == Color::BLACK)
        key_ ^= Zobrist::BlackToMoveZobrist();

    // - - 0 1
    std::string temp;
    for (int i = 0; i < 4; ++i)
    {
        ss >> temp;
    }

    // moves xxxx xxxx ...
    std::string move;
    if (ss >> move)
    {
        past_positions_.clear();
        past_positions_.insert(key_);
        while (ss >> move)
        {
            UndoInfo undoInfo;
            MakeMove(String2Move(move), undoInfo);
            past_positions_.insert(key_);
        }
    }

};

std::string Position::GenerateFen() const
{
    std::string fen;
    std::string seg;
    for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST)
    {
        if (s != SQ_A0 && s % 9 == 0)
        {
            fen = ("/" + seg) + fen;
            seg.clear();
        }
        if (board_[s] != Piece::NO_PIECE)
            seg.push_back(CharFromPiece(board_[s]));
        else
        {
            if (seg.back() > '0' && seg.back() <= '9')
                seg.back() += 1;
            else
                seg.push_back('1');
        }
    }
    fen = seg + fen;
    fen.push_back(' ');
    fen.push_back(side_to_move_ == Color::RED ? 'w' : 'b');
    fen += " - - 0 1";
    return fen;
}

void Position::DisplayBoard(bool reverse) const
{
    std::string boardStr;
    std::string segStr;

    for (Square s = SQ_A0; s < SQ_NUM; s += SQ_EAST)
    {
        if (s % 9 == 0)
        {
            boardStr = segStr + boardStr;
            segStr.clear();
        }
        segStr.push_back(CharFromPiece(board_[s]));
    }
    boardStr = segStr + boardStr;

    std::stringstream ss;
    ss << std::endl;
    ss << "  +---+---+---+---+---+---+---+---+---+" << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        ss << (9 - i) << " |";
        for (int j = 0; j < 9; ++j)
        {
            ss << " " << (boardStr[i * 9 + j] == '0' ? ' ' : boardStr[i * 9 + j]) << " " << "|";
        }
        ss << std::endl;
        ss << "  +---+---+---+---+---+---+---+---+---+" << std::endl;
    }
    ss << "  ";
    for (int j = 0; j < 9; ++j)
    {
        ss << "  " << static_cast<char>('a' + j) << " ";
    }
    ss << std::endl;

    std::string board = ss.str();
    if (reverse)
        std::reverse(board.begin(), board.end());
    std::cout << board;
}

Square Position::KingSquare(Color c) const
{
    Bitboard b = Pieces(c, PieceType::KING);
    assert(b);
    return PopLSB(b);
}

bool Position::IsChecked(Color c) const
{
    Square ksq = KingSquare(c);

    return AttackBB<PieceType::ROOK>(ksq, AllPieces()) & Pieces(!c, PieceType::ROOK)
        || AttackBB<PieceType::CANNON>(ksq, AllPieces()) & Pieces(!c, PieceType::CANNON)
        || AttackBB<PieceType::KNIGHT_TO>(ksq, AllPieces()) & Pieces(!c, PieceType::KNIGHT)
        || AttackBB<PieceType::ROOK>(ksq, AllPieces()) & Pieces(!c, PieceType::KING)
        || AttackBB<PieceType::PAWN_TO>(ksq, !c) & Pieces(!c, PieceType::PAWN);
}

Bitboard Position::CheckersBB(Square ksq, Color kc, Bitboard occupancy) const
{
    return ( (AttackBB<PieceType::ROOK>(ksq, occupancy) & Pieces(PieceType::ROOK))
           | (AttackBB<PieceType::CANNON>(ksq, occupancy) & Pieces(PieceType::CANNON))
           | (AttackBB<PieceType::KNIGHT_TO>(ksq, occupancy) & Pieces(PieceType::KNIGHT))
           | (AttackBB<PieceType::ROOK>(ksq, occupancy) & Pieces(PieceType::KING))  // king
           | (AttackBB<PieceType::PAWN_TO>(ksq, !kc) & Pieces(!kc, PieceType::PAWN))
           ) & Pieces(!kc);
}

bool Position::IsLegalMove(Move move) const
{
    auto from = move.MoveFrom(), to = move.MoveTo();
    auto occupancy = (AllPieces() ^ SquareBB(from)) | SquareBB(to);
    if (TypeOfPiece(piece_from_square(from)) == PieceType::KING)
        return !CheckersBB(to, side_to_move_, occupancy);
    // checker in "to" position is attacked
    return !(CheckersBB(KingSquare(side_to_move_), side_to_move_, occupancy) & ~SquareBB(to));
}

Piece Position::PieceFromChar(char c) const
{
    switch (c)
    {
    case 'R':
        return Piece::W_ROOK;
    case 'r':
        return Piece::B_ROOK;
    case 'N':
        return Piece::W_KNIGHT;
    case 'n':
        return Piece::B_KNIGHT;
    case 'B':
        return Piece::W_BISHOP;
    case 'b':
        return Piece::B_BISHOP;
    case 'A':
        return Piece::W_ADVISOR;
    case 'a':
        return Piece::B_ADVISOR;
    case 'K':
        return Piece::W_KING;
    case 'k':
        return Piece::B_KING;
    case 'C':
        return Piece::W_CANNON;
    case 'c':
        return Piece::B_CANNON;
    case 'P':
        return Piece::W_PAWN;
    case 'p':
        return Piece::B_PAWN;
    default:
        break;
    }
    return Piece::NO_PIECE;
}

char Position::CharFromPiece(Piece piece) const
{
    switch (piece)
    {
    case Piece::W_ROOK:
        return 'R';
    case Piece::B_ROOK:
        return 'r';
    case Piece::W_KNIGHT:
        return 'N';
    case Piece::B_KNIGHT:
        return 'n';
    case Piece::W_BISHOP:
        return 'B';
    case Piece::B_BISHOP:
        return 'b';
    case Piece::W_ADVISOR:
        return 'A';
    case Piece::B_ADVISOR:
        return 'a';
    case Piece::W_KING:
        return 'K';
    case Piece::B_KING:
        return 'k';
    case Piece::W_CANNON:
        return 'C';
    case Piece::B_CANNON:
        return 'c';
    case Piece::W_PAWN:
        return 'P';
    case Piece::B_PAWN:
        return 'p';
    default:
        break;
    }
    return '0';
}

void Position::put_piece(Piece p, Square s)
{
    by_type_bb_[NUM(TypeOfPiece(board_[s]))] ^= SquareBB(s);
    by_type_bb_[NUM(TypeOfPiece(p))] |= SquareBB(s);
    if (board_[s] != Piece::NO_PIECE)
        by_color_bb_[NUM(ColorOfPiece(board_[s]))] ^= SquareBB(s);
    if (p != Piece::NO_PIECE)
        by_color_bb_[NUM(ColorOfPiece(p))] |= SquareBB(s);
    --piece_count_[NUM(board_[s])];
    ++piece_count_[NUM(p)];
    board_[s] = p;
}

void Position::remove_piece(Square s)
{
    put_piece(Piece::NO_PIECE, s);
}

void Position::move_piece(Square from, Square to)
{
    Piece p = board_[from];
    remove_piece(from);
    put_piece(p, to);
}