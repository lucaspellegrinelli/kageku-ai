#include "move_list.h"

MoveList::MoveList(){
  this->count = 0;
}

void MoveList::add_quiet_move(Move move){
  this->moves[this->count++] = move;
}

void MoveList::add_capture_move(Move move){
  this->moves[this->count++] = move;
}

void MoveList::add_new_piece_move(Move move){
  this->add_quiet_move(move);
}

void MoveList::print(){
  for(int i = 0; i < this->count; i++){
    Move move = this->moves[i];
    std::cout << "Move: #" << (i + 1) << ": " << move.get_repr() << " [" << (move.is_move() ? 'M' : 'A') << "]" << " (score: " << move.get_score() << ")" << std::endl;
  }

  std::cout << "Move list size: " << this->count << std::endl;
}
