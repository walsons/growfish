"""
This file is used to generate best move for game start, growfish 
currently doesn't perform well at game start, we use another UCI 
engine(pikafish) to make a book for game start positions.

Creating process in cpp is not same across from the different platform,
We use a Python file to create the book.
"""

import subprocess
import position


engine_name = "pikafish.exe"
nnue_name = "pikafish.nnue"
search_depth = 8

def start_engine(engine_path):
    return subprocess.Popen(engine_path, stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)

def send_command(engine, command):
    engine.stdin.write(command + '\n')
    engine.stdin.flush()

def read_response(engine):
    while True:
        response = engine.stdout.readline().strip()
        # print(response)
        if response == 'uciok' or response == 'readyok':
            break
    return None

def read_best_move(engine):
    while True:
        response = engine.stdout.readline().strip()
        # print(response)
        if response.startswith('bestmove'):
            return response[9: 13]

def read_pv_moves(engine):
    pv_moves = []
    while True:
        response = engine.stdout.readline().strip()
        # print(response)
        if response.startswith('info depth ' + str(search_depth)):
            pos = response.find(" pv ")
            pv_moves.append(response[pos + 4: pos + 8])
        # Exit when response.startswith('bestmove')
        elif response.startswith('bestmove'):
            return pv_moves

def read_fen(engine):
    while True:
        response = engine.stdout.readline().strip()
        # print(response)
        if response.startswith("Fen: "):
            return response[5:]

def position_key(fen):
    p = position.Position(fen)
    return p.key


fen_books = {}
key_books = {}
stop_ply = 6

def move_forward(engine, ply):
    if ply >= stop_ply:
        return

    send_command(engine, 'd')
    fen = read_fen(engine)

    # pruning for accelerate
    # if fen in fen_books:
    #     return

    send_command(engine, 'go depth ' + str(search_depth))
    best_move = read_best_move(engine)

    fen_books[fen] = best_move
    key_books[position_key(fen)] = best_move

    # enemy move
    send_command(engine, 'position fen ' + fen + " moves " + best_move)
    send_command(engine, 'd')
    fen = read_fen(engine)

    send_command(engine, 'go depth ' + str(search_depth))
    pv_moves = read_pv_moves(engine)
    for move in pv_moves:
        send_command(engine, 'position fen ' + fen + " moves " + move)
        move_forward(engine, ply + 1)
    

def write_book(engine):
    """ First, collect red best move """
    send_command(engine, 'ucinewgame')
    send_command(engine, 'position startpos')

    send_command(engine, 'd')
    fen = read_fen(engine)

    send_command(engine, 'go depth ' + str(search_depth))
    best_move = read_best_move(engine)

    fen_books[fen] = best_move
    key_books[position_key(fen)] = best_move

    # black move
    send_command(engine, 'position fen ' + fen + " moves " + best_move)
    send_command(engine, 'd')
    fen = read_fen(engine)

    send_command(engine, 'go depth ' + str(search_depth))
    pv_moves = read_pv_moves(engine)
    for move in pv_moves:
        send_command(engine, 'position fen ' + fen + " moves " + move)
        move_forward(engine, 0)

    """ Second, collect black best move """
    send_command(engine, 'ucinewgame')
    send_command(engine, 'position startpos')

    send_command(engine, 'd')
    start_fen = read_fen(engine)

    send_command(engine, 'go depth ' + str(search_depth))
    pv_moves = read_pv_moves(engine)
    # Here are first moves for red
    for move in pv_moves:
        send_command(engine, 'position fen ' + start_fen + " moves " + move)
        move_forward(engine, 0)


def main():
    engine = start_engine(engine_name)
    
    send_command(engine, 'uci')
    read_response(engine)
    
    send_command(engine, 'setoption name Threads value 1')
    send_command(engine, 'setoption name Hash value 256')
    # set diversity to 10 for each step
    send_command(engine, 'setoption name MultiPV value 10')
    
    send_command(engine, 'isready')
    read_response(engine)
    
    write_book(engine)

    import json
    filename = 'fendata.json'
    with open(filename, 'w') as f:
        json.dump(fen_books, f, indent=4)
    filename = 'keydata.json'
    with open(filename, 'w') as f:
        json.dump(key_books, f, indent=4)
    
    send_command(engine, 'quit')
    engine.terminate()


if __name__ == '__main__':
    main()