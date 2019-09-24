#ifndef MOVE_LIST_H
#define MOVE_LIST_H

#include "move.h"
#include "defs.h"

class MoveList{
public:
  Move moves[MAX_POSITION_MOVES];
  int count;

  MoveList();

  void add_quiet_move(int move);
  void add_capture_move(int move);

  void print();
};

#endif
