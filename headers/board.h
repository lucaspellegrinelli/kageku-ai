#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <iomanip>

#include "stdlib.h"

#include "defs.h"
#include "macros.h"

#include "move.h"
#include "move_list.h"
#include "bitboard.h"

class Board{
private:
  // Board represented as an array of 120 positions (main 64x64 board plus edges)
  int pieces[BOARD_SQ_NUM];

  // Key representation of each piece in each square - Initialized on constructor
  U64 piece_keys[13][120];

  // Each U64 will store a value of 0 or 1 for each bit of the board corresponding
  // to having a pawn there or not. One U64 number for each of the colors
  Bitboard pawns[COLOR_COUNT + 1];

  // Position of the kings for each color
  int king_square[COLOR_COUNT];

  // Stores which side will be playing now
  int side_to_move;

  // Key representation of side to play - Initialized on constructor
  U64 side_key;

  // Counts the move so there's the fifty move rule
  int fifty_move_counter;

  // Current number of half moves
  int ply;
  int history_ply;

  // Key for each board position
  U64 position_key;

  // How many of each of the pieces are there on the board
  int piece_count[PIECE_TYPES];

  // How many of each of the pieces (except pawns) are there on the board
  int not_pawn_piece_count[COLOR_COUNT];

  // How many Queens and Rooks are there for each team
  int major_piece_count[COLOR_COUNT];

  // How many Bishops and Knights are there for each team
  int minor_piece_count[COLOR_COUNT];

  // Material count for each team
  int material[COLOR_COUNT];

  // Info about the moves played previously
  MoveInfo history[MAX_GAME_MOVES];

  // Piece list (10 = suppose you start with 2 rooks and promote all 8 pawns to rooks)
  // piece_list[wN][0] = E1, piece_list[wN][1] = D4 ...
  int piece_list[PIECE_TYPES][10] = {
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE},
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE},
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE},
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE},
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE},
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE},
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE},
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE},
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE},
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE},
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE},
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE},
    {NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE, NO_SQUARE}
  };

public:
  Board();

  MoveList generate_all_moves();

  bool is_square_attacked(int sq, int side);

  void update_lists_material();

  bool check_board();
  void print();

  void set_fen(char *fen);
  void reset();

  U64 generate_position_key();

  void initialize_side_key();
  void initialize_piece_keys();
};

#endif
