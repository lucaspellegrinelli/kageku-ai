#include "move_list.h"

MoveList::MoveList(){
  this->count = 0;
}

void MoveList::add_quiet_move(int from, int to){
  this->moves[this->count++] = Move(from, to, EMPTY);
}

void MoveList::add_capture_move(int from, int to, int captured){
  this->moves[this->count++] = Move(from, to, captured);
}

void MoveList::print(){
  for(int i = 0; i < this->count; i++){
    Move move = this->moves[i];
    std::cout << "Move: #" << (i + 1) << ": " << move.get_repr() << " (score: " << move.get_score() << ")" << std::endl;
  }

  std::cout << "Move list size: " << this->count << std::endl;
}
