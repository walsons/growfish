#include "position.h"

#include <iostream>
#include <sstream>

#include "bitboard.h"
#include "magic.h"

void Position::SetPosition(const std::string& fen)
{
    std::memset(by_type_bb_, 0, sizeof(Bitboard) * NUM(PieceType::PIECE_TYPE_NUM));
    std::memset(by_color_bb_, 0, sizeof(Bitboard) * NUM(Color::COLOR_NUM));
    std::memset(piece_count_, 0, sizeof(int) * NUM(Piece::PIECE_NUM));

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
        while (ss >> move)
        {
            UndoInfo undoInfo;
            MakeMove(String2Move(move), undoInfo);
        }
    }
};

std::string Position::GenerateFen()
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
}

void Position::SimpleMakeMove(Move move, UndoInfo& undoInfo)
{
    auto from = MoveFrom(move);
    auto to = MoveTo(move);

    undoInfo = { move, board_[to] };
    move_piece(from, to);
    // undoInfo = { move, board_[to] };
    // board_[to] = board_[from];
    // board_[from] = Piece::NO_PIECE;
}

void Position::SimpleUndoMove(const UndoInfo& undoInfo)
{
    auto from = MoveFrom(undoInfo.move);
    auto to = MoveTo(undoInfo.move);

    // board_[from] = board_[to];
    // board_[to] = undoInfo.piece;
    move_piece(to, from);
    put_piece(undoInfo.piece, to);
}

void Position::DisplayBoard()
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

    std::cout << "  +---+---+---+---+---+---+---+---+---+" << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << (9 - i) << " |";
        for (int j = 0; j < 9; ++j)
        {
            std::cout << " " << (boardStr[i * 9 + j] == '0' ? ' ' : boardStr[i * 9 + j]) << " " << "|";
        }
        std::cout << std::endl;
        std::cout << "  +---+---+---+---+---+---+---+---+---+" << std::endl;
    }
    std::cout << "  ";
    for (int j = 0; j < 9; ++j)
    {
        std::cout << "  " << static_cast<char>('a' + j) << " ";
    }
    std::cout << std::endl;
}

Square Position::KingSquare(Color c)
{
    Bitboard b = Pieces(c, PieceType::KING);
    assert(b);
    return PopLSB(b);
}

bool Position::IsEnemyChecked()
{
    return IsChecked(side_to_move_ == Color::RED ? Color::BLACK : Color::RED);
}

bool Position::IsSelfChecked()
{
    return IsChecked(side_to_move_);
}

bool Position::IsChecked(Color c)
{
    Square kPos = KingSquare(c);

    /***** Rook *****/
    if (Attack<PieceType::ROOK>(AllPieces(), kPos) & Pieces(!c, PieceType::ROOK))
        return true;

    /***** Cannon *****/
    if (Attack<PieceType::CANNON>(AllPieces(), kPos) & Pieces(!c, PieceType::CANNON))
        return true;

    /***** Knight *****/
    if (Attack<PieceType::KNIGHT_TO>(AllPieces(), kPos) & Pieces(!c, PieceType::KNIGHT))
        return true;

    /**** King *****/
    if (Attack<PieceType::ROOK>(AllPieces(), kPos) & Pieces(!c, PieceType::KING))
        return true;

    /***** Pawn *****/
    if (c == Color::RED)
    {
        if (Attack<PieceType::PAWN_TO, Color::BLACK>(kPos) & Pieces(Color::BLACK, PieceType::PAWN))
            return true;
    }
    else
    {
        if (Attack<PieceType::PAWN_TO, Color::RED>(kPos) & Pieces(Color::RED, PieceType::PAWN))
            return true;
    }

    return false;
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