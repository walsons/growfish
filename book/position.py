def limit_to_64bit(value):
    return value & ((1 << 63) - 1)

class PRNG:
    def __init__(self, seed):
        self.s = seed

    def rand64(self):
        self.s ^= (self.s >> 12)
        self.s ^= (self.s << 25)
        self.s ^= (self.s >> 27)
        return limit_to_64bit(self.s * 2685821657736338717)

    def rand(self, type_func):
        return type_func(self.rand64())
    

class Zobrist:
    def __init__(self):
        self.piece_square_zobrist = []
        self.black_to_move_zobrist = None

        rng = PRNG(1)
        for i in range(15):
            sa = []
            for s in range(90):
                sa.append(rng.rand64())
            self.piece_square_zobrist.append(sa)
        self.black_to_move_zobrist = rng.rand64()


from enum import Enum

class Piece(Enum):
    W_PAWN = 0
    W_ADVISOR = 1
    W_BISHOP = 2
    W_KNIGHT = 3
    W_CANNON = 4
    W_ROOK = 5
    W_KING = 6

    B_PAWN = 7
    B_ADVISOR = 8
    B_BISHOP = 9
    B_KNIGHT = 10
    B_CANNON = 11
    B_ROOK = 12
    B_KING = 13

    NO_PIECE = 14

    PIECE_NUM = 15


class Position:
    # only accept fen without moves
    def __init__(self, fen):
        self.key = 0
        segments = fen.split()
        segments = [item.strip() for item in segments]
        board_str = ""
        seg_str = ""
        for c in segments[0]:
            if c == " ":
                break
            elif c in "123456789":
                seg_str += int(c) * '0'
            elif c == "/":
                board_str = seg_str + board_str
                seg_str = ""
                continue
            else:
                seg_str += c
        board_str = seg_str + board_str

        zob = Zobrist()
        for s in range(90):
            self.key ^= zob.piece_square_zobrist[self.__piece_from_char(board_str[s]).value][s]
        if segments[1] == "b":
            self.key ^= zob.black_to_move_zobrist
    
    def __piece_from_char(self, c):
        piece_map = {
            'R': Piece.W_ROOK,
            'r': Piece.B_ROOK,
            'N': Piece.W_KNIGHT,
            'n': Piece.B_KNIGHT,
            'B': Piece.W_BISHOP,
            'b': Piece.B_BISHOP,
            'A': Piece.W_ADVISOR,
            'a': Piece.B_ADVISOR,
            'K': Piece.W_KING,
            'k': Piece.B_KING,
            'C': Piece.W_CANNON,
            'c': Piece.B_CANNON,
            'P': Piece.W_PAWN,
            'p': Piece.B_PAWN,
        }
        return piece_map.get(c, Piece.NO_PIECE)



