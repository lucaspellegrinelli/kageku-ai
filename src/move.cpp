#include "move.h"

Move::Move(){
  this->move = 0;
  this->score = 0;
}

Move::Move(int move){
  this->move = move;
  this->score = 0;
}

Move::Move(int move, int score){
  this->move = move;
  this->score = score;
}

Move::Move(int from, int to, int captured){
  this->move = (from) | (to << 7) | (captured << 14);
  this->score = 0;
}

Move::Move(int from, int to, int captured, int score){
  this->move = (from) | (to << 7) | (captured << 14);
  this->score = score;
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

int Move::get_score(){
  return this->score;
}

std::string Move::get_repr(){
  int from = this->get_from();
  int to = this->get_to();
  std::string repr = "";
  repr += 'a' + SQUARE_FILE[from];
  repr += '1' + SQUARE_RANK[from];
  repr += 'a' + SQUARE_FILE[to];
  repr += '1' + SQUARE_RANK[to];
  return repr;
}

void Move::print(){
  std::ostream movecout(std::cout.rdbuf());
  movecout << this->get_repr() << std::endl;
}
