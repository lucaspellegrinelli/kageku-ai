#include "move_list.h"

MoveList::MoveList(){
  this->count = 0;
}

void MoveList::add_quiet_move(int from, int to){
  ASSERT(IS_SQUARE_ON_BOARD(from));
  ASSERT(IS_SQUARE_ON_BOARD(to));

  int move_id = Move::create_move(from, to);
  this->moves[this->count++] = Move(move_id);
}

void MoveList::add_capture_move(int from, int to, int captured){
  ASSERT(IS_SQUARE_ON_BOARD(from));
  ASSERT(IS_SQUARE_ON_BOARD(to));
  ASSERT(IS_PIECE_VALID_OR_EMPTY(captured));

  int move_id = Move::create_move(from, to, captured);
  this->moves[this->count++] = Move(move_id);
}

void MoveList::add_new_piece_move(Move move){
  this->moves[this->count++] = move;
}

void MoveList::print(){
  for(int i = 0; i < this->count; i++){
    Move move = this->moves[i];
    std::cout << "Move: #" << (i + 1) << ": " << move.get_repr() << " [" << (move.is_move() ? 'M' : 'A') << "]" << " (score: " << move.get_score() << ")" << std::endl;
  }

  std::cout << "Move list size: " << this->count << std::endl;
}
