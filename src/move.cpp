#include "move.h"

Move::Move(){
  this->score = 0;
  this->move_count = 0;
}

Move::Move(int move){
  this->move[0] = move;
  this->score = 0;
  this->move_count = 1;
}

Move::Move(int move, int score){
  this->move[0] = move;
  this->score = score;
  this->move_count = 1;
}

int Move::create_move(int from, int to){
  return (from) | (to << 7) | (EMPTY << 14) | (1 << 15);
}

int Move::create_move(int from, int to, int captured){
  return (from) | (to << 7) | (captured << 14) | (1 << 15);
}

int Move::create_add(int square, int piece){
  return (square) | (piece << 7) | (1 << 15);
}

bool Move::is_move(){
  if(this->move_count > 0){
    return (this->move[0] >> 15) & 0x1;
  }

  return false;
}

bool Move::is_add(){
  if(this->move_count > 0){
    return ~((this->move[0] >> 15) & 0x1);
  }

  return false;
}

void Move::add_move(int move){
  ASSERT(this->is_add());
  this->move[this->move_count++] = move;
}

int Move::get_from(){
  ASSERT(this->is_move());
  return this->move[0] & 0x7F;
}

int Move::get_to(){
  ASSERT(this->is_move());
  return (this->move[0] >> 7) & 0x7F;
}

int Move::get_captured(){
  ASSERT(this->is_move());
  return (this->move[0] >> 14) & 0xF;
}

int Move::get_add_square(int i){
  ASSERT(this->is_add());
  return this->move[i] & 0x7F;
}

int Move::get_add_piece(int i){
  ASSERT(this->is_add());
  return (this->move[i] >> 7) & 0xF;
}

int Move::get_score(){
  return this->score;
}

std::string Move::get_repr(){
  if(this->is_move()){
    int from = this->get_from();
    int to = this->get_to();
    std::string repr = "";
    repr += 'a' + SQUARE_FILE[from];
    repr += '1' + SQUARE_RANK[from];
    repr += 'a' + SQUARE_FILE[to];
    repr += '1' + SQUARE_RANK[to];
    return repr;
  }else{
    std::string repr = "";
    for(int i = 0; i < this->move_count; i++){
      int sq = this->get_add_square(i);
      int piece = this->get_add_piece(i);
      repr += 'a' + SQUARE_FILE[sq];
      repr += '1' + SQUARE_RANK[sq];
      repr += PIECE_CHARS[piece];
    }

    return repr;
  }
}

void Move::print(){
  std::ostream movecout(std::cout.rdbuf());
  movecout << this->get_repr() << std::endl;
}
