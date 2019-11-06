#include <iostream>
#include <string>

#include "defs.h"
#include "macros.h"

#include "board.h"
#include "ai.h"
#include "move.h"
#include "bitboard.h"

int main(){
  Board *b = new Board();
  SearchInfo *info = new SearchInfo();

  while(true){
    std::cout << std::endl;
    b->print();
    std::cout << std::endl;

    std::string in_move;
    std::cout << "Type in your move > ";
    std::cin >> in_move;

    if(in_move == "take" || in_move == "t"){
      b->take_move();
    }else if(in_move == "quit" || in_move == "q"){
      break;
    }else if(in_move == "search" || in_move == "s"){
      info->depth = 5;
      AI::search_position(b, info);
    }else if(in_move == "showmoves" || in_move == "sm"){
      MoveList all_moves = b->generate_all_moves();
      for(int i = 0; i < all_moves.count; i++){
        std::cout << "Move #" << i << ": " << all_moves.moves[i].get_repr() << std::endl;
      }
    }else{
      Move move = Move::parse_move(in_move);

      if(!move.is_valid()) continue;

      MoveList all_moves = b->generate_all_moves();
      if(all_moves.is_move_in_list(move)){
        b->make_move(move);
      }else{
        std::cout << "Move not parsed" << std::endl;
      }
    }
  }

  delete b;
  return 0;
}
