#include "move.h"

Move::Move(){
  this->move = 0;
  this->score = 0;
}

Move::Move(int move){
  this->move = move;
  this->score = 0;
}

int Move::get_from(){
  return this->move & 0x7F;
}

int Move::get_to(){
  return (this->move >> 7) & 0x7F;
}

int Move::get_captured(){
  return (this->move >> 14) & 0xF;
}

void Move::print(){
  std::ostream movecout(std::cout.rdbuf());

  int from = this->get_from();
  int to = this->get_to();

  char ff = 'a' + SQUARE_FILE[from];
  char fr = '1' + SQUARE_RANK[from];
  char tf = 'a' + SQUARE_FILE[to];
  char tr = '1' + SQUARE_RANK[to];

  movecout << ff << fr << tf << tr << std::endl;
}
