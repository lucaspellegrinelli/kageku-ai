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

  Move a = Move(Move::create_add(36, wQ));
  std::cout << "Test: " << a.get_add_square(0) << " " << a.get_add_piece(0) << std::endl;

  MoveList move_list = b.generate_all_moves();

  for(int i = 0; i < move_list.count; i++){
    Move move = move_list.moves[i];
    std::cout << "Found " << move.get_repr() << std::endl;
    b.make_move(move);
    b.print();
    std::cout << std::endl;
    b.take_move();
  }

  return 0;
}
