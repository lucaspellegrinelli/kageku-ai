#include <iostream>

#include "defs.h"
#include "macros.h"

#include "board.h"
#include "move.h"
#include "bitboard.h"

int main(){
  Board b;
  b.print();

  b.generate_all_moves().print();

  return 0;
}
