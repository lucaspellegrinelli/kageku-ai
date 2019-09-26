#include <iostream>

#include "defs.h"
#include "macros.h"

#include "board.h"
#include "move.h"
#include "bitboard.h"

int main(){
  Board b;
  b.print();

  std::cout << std::endl;

  b.generate_all_moves().print();

  return 0;
}
