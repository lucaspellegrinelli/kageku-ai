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

    std::string in_move;
    std::cout << "Type in your move > ";
    std::cin >> in_move;

    if(in_move == "take" || in_move == "t"){
      b.take_move();
    }else if(in_move == "quit" || in_move == "q"){
      break;
    }else{
      Move move = Move::parse_move(in_move);

      if(!move.is_valid()) continue;

      MoveList all_moves = b.generate_all_moves();
      if(all_moves.is_move_in_list(move)){
        b.make_move(move);
      }
    }
  }

  return 0;
}
