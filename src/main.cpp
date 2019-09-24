#include <iostream>

#include "defs.h"
#include "macros.h"

#include "board.h"
#include "move.h"
#include "bitboard.h"

int main(){
  Board b;
  b.print();

  int from_sq = H2;
  int to_sq = H3;
  int captured = wR;

  int move = (from_sq) | (to_sq << 7) | (captured << 14);
  Move m(move);
  m.print();

  return 0;
}
