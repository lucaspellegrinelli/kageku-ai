#include <iostream>
#include <string>

#include "defs.h"
#include "macros.h"

#include "board.h"
#include "move.h"
#include "bitboard.h"

int main(){
  Board *b = new Board();

  while(true){
    b->print();
    std::cout << std::endl;

    std::string in_move;
    std::cout << "Type in your move > ";
    std::cin >> in_move;

    if(in_move == "take" || in_move == "t"){
      b->take_move();
    }else if(in_move == "quit" || in_move == "q"){
      break;
    }else if(in_move == "showpv"){
      int max = b->get_pv_line(4);
      std::cout << "PV Line of " << max << " moves: ";
      for(int i = 0; i < max; i++){
        std::cout << b->get_from_pv_array(i).get_repr() << " ";
      }
      std::cout << std::endl;
    }else{
      Move move = Move::parse_move(in_move);

      if(!move.is_valid()) continue;

      MoveList all_moves = b->generate_all_moves();
      if(all_moves.is_move_in_list(move)){
        b->add_move_to_hash_table(move);
        b->make_move(move);
      }else{
        std::cout << "Move not parsed" << std::endl;
      }
    }
  }

  return 0;
}
