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

bool MoveList::is_move_in_list(Move move){
  for(int i = 0; i < this->count; i++){
    Move curr_move = this->moves[i];
    if(curr_move.get_move_size() != move.get_move_size()) continue;

    bool found_diff = false;
    for(int j = 0; j < curr_move.get_move_size(); j++){
      if(curr_move.get_move(j) != move.get_move(j)){
        found_diff = true;
        break;
      }
    }

    if(!found_diff) return true;
  }

  return false;
}

void MoveList::print(){
  for(int i = 0; i < this->count; i++){
    Move move = this->moves[i];
    std::cout << "Move: #" << (i + 1) << ": " << move.get_repr() << " [" << (move.is_move() ? 'M' : 'A') << "]" << " (score: " << move.get_score() << ")" << std::endl;
  }

  std::cout << "Move list size: " << this->count << std::endl;
}
