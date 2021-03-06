#ifndef MOVE_LIST_H
#define MOVE_LIST_H

#include "defs.h"
#include "macros.h"

#include "move.h"

class MoveList{
public:
  Move moves[MAX_POSITION_MOVES];
  int count;

  MoveList();

  void add_quiet_move(Move move);
  void add_capture_move(Move move);
  void add_new_piece_move(Move move);

  void reorder_next_move(int move_index);

  void reverse();

  bool is_move_in_list(Move move, bool strict=true);
  Move get_aprox_move_in_list(Move move);

  void print();
};

#endif
