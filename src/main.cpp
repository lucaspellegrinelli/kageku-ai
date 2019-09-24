#include <iostream>

#include "defs.h"
#include "macros.h"

#include "board.h"
#include "move.h"
#include "bitboard.h"

int main(){
  Board b;
  b.print();

  Move m(H2, H3, EMPTY);
  m.print();

  return 0;
}
