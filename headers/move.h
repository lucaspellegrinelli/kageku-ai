#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <string>

#include "defs.h"
#include "macros.h"

// Class responsible for storing the states of the game before a move for
// undoing it
class MoveInfo{
  int move[MAX_POSITION_MOVES];
  int fifty_move_counter;
  U64 position_key;
};

class Move{
private:
  /*
    Stores all info about the move in the 32 bits available

    MOVE -
    7 bits -> "From square" -> 2^7 = 64 = Number of squares in the board
    0000 0000 0000 0000 0000 0000 0111 1111 -> [0x7F]

    7 bits -> "To square"   -> 2^7 = 64 = Number of squares in the board
    0000 0000 0000 0000 0011 1111 1000 0000 -> [>> 7, 0x7F]

    4 bits -> Piece the move captured -> 2^4 = 16 > Number of pieces (12, 6 for each color)
    0000 0000 0000 0011 1100 0000 0000 0000 -> [>> 14, 0xF]

    1 bit -> Is move? -> 2^1 = 2 = Number of options
    0000 0000 0000 0100 0000 0000 0000 0000 -> [>> 15, 0x1]

    ADD -
    7 bits -> "Square" -> 2^7 = 64 = Number of squares in the board
    0000 0000 0000 0000 0000 0000 0111 1111 -> [0x7F]

    4 bits -> Piece added -> 2^4 = 16 > Number of pieces (12, 6 for each color)
    0000 0000 0000 0000 0000 0111 1000 0000 -> [>> 7, 0xF]

    1 bit -> Is move? -> 2^1 = 2 = Number of options
    0000 0000 0000 0100 0000 0000 0000 0000 -> [>> 15, 0x1]
  */
  int move[MAX_POSITION_MOVES];

  // Number of actions in this move
  int move_count = 0;

  // Used for move ordering in the search
  int score;

public:
  Move();
  Move(int move);
  Move(int move, int score);

  static int create_move(int from, int to);
  static int create_move(int from, int to, int captured);
  static int create_add(int square, int piece);

  void add_move(int move);

  bool is_move();
  bool is_add();

  int get_from();
  int get_to();
  int get_captured();
  int get_score();

  int get_add_square(int i);
  int get_add_piece(int i);

  std::string get_repr();
  void print();
};

#endif
