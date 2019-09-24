#include <iostream>

#include "defs.h"
#include "macros.h"

#include "board.h"
#include "bitboard.h"

int main(){
  Board b;
  b.print();

  ASSERT(b.check_board())

  return 0;
}