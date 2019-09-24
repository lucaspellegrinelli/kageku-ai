#include "move_list.h"

MoveList::MoveList(){
  this->count = 0;
}

void MoveList::add_quiet_move(int move){
  this->moves[this->count++] = Move(move, 0);
}

void MoveList::add_capture_move(int move){
  this->moves[this->count++] = Move(move, 0);
}
