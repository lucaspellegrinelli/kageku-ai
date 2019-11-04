#include <iostream>
#include <string>

#include "defs.h"
#include "macros.h"

#include "board.h"
#include "move.h"
#include "bitboard.h"

int main(){
  Board b;

  while(true){
    b.print();
    std::cout << std::endl;

    MoveList move_list = b.generate_all_moves();
    move_list.print();

    std::string in_move;
    std::cout << "Type in your move: ";
    std::cin >> in_move;

    if(in_move == "t"){
      std::cout << "You took back the last move" << std::endl;
      b.take_move();
    }else{
      Move move = Move::parse_move(in_move);
      std::cout << "You typed: "<< move.get_repr() << std::endl;
      b.make_move(move);
    }
  }

  return 0;
}
