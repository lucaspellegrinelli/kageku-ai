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

void MoveList::reverse(){
  Move moves_cpy[MAX_POSITION_MOVES];
  for(int i = 0; i < this->count; i++){
    moves_cpy[i] = this->moves[i];
  }

  for(int i = this->count - 1; i >= 0; i--){
    this->moves[this->count - i - 1] = moves_cpy[i];
  }
}

void MoveList::reorder_next_move(int move_index){
  int best_score = 0;
  int best_index = move_index;

  for(int i = 0; i < this->count; i++){
    if(this->moves[i].get_score() > best_score){
      best_score = this->moves[i].get_score();
      best_index = i;
    }
  }

  Move t_move = this->moves[move_index];
  this->moves[move_index] = this->moves[best_index];
  this->moves[best_index] = t_move;
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
