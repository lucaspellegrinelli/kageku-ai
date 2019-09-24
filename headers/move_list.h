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

  void add_quiet_move(int from, int to);
  void add_capture_move(int from, int to, int captured);

  void print();
};

#endif
