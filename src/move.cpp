#include "move.h"

Move::Move(){
  this->score = 0;
  this->move_count = 0;
  this->valid = true;
}

Move::Move(int move){
  this->move[0] = move;
  this->score = 0;
  this->move_count = 1;
  this->valid = true;
}

Move::Move(int move, int score){
  this->move[0] = move;
  this->score = score;
  this->move_count = 1;
  this->valid = true;
}

int Move::create_move(int from, int to){
  return (from) | (to << 7) | (EMPTY << 14) | (1 << 18);
}

int Move::create_move(int from, int to, int captured){
  return (from) | (to << 7) | (captured << 14) | (1 << 18);
}

int Move::create_add(int square, int piece){
  return (square) | (piece << 7) | (0 << 18);
}

Move Move::unvalid_move(){
  Move m;
  m.set_valid(false);
  return m;
}

bool Move::is_equal(Move move){
  if(this->move_count != move.get_move_size()) return false;
  for(int i = 0; i < this->move_count; i++){
    if(this->move[i] != move.get_move(i)) return false;
  }

  return true;
}

Move Move::parse_move(std::string move_str){
  if(move_str.size() == 4){
    if((move_str[0] < 'a' && move_str[0] > 'h') ||
       (move_str[2] < 'a' && move_str[2] > 'h') ||
       (move_str[1] < '1' && move_str[1] > '8') ||
       (move_str[3] < '1' && move_str[3] > '8')){

      Move move;
      move.valid = false;
      return move;
    }

    int from = FILE_RANK_TO_SQUARE(move_str[0] - 'a', move_str[1] - '1');
    int to = FILE_RANK_TO_SQUARE(move_str[2] - 'a', move_str[3] - '1');

    ASSERT(IS_SQUARE_ON_BOARD(from));
    ASSERT(IS_SQUARE_ON_BOARD(to))

    Move move(Move::create_move(from, to));
    return move;
  }else if(move_str.size() > 0 && move_str.size() % 3 == 0){
    Move move;
    for(unsigned int i = 0; i < move_str.size(); i += 3){
      std::string submove_str = move_str.substr(i, 3);

      if((submove_str[0] < 'a' && submove_str[0] > 'h') ||
         (submove_str[1] < '1' && submove_str[1] > '8') ||
         (!IS_PIECE_REPR_VALID(submove_str[2]))){

        Move move;
        move.valid = false;
        return move;
      }

      int add_square = FILE_RANK_TO_SQUARE(submove_str[0] - 'a', submove_str[1] - '1');
      int add_piece = -1;

      if(submove_str[2] == 'P') add_piece = wP;
      else if(submove_str[2] == 'N') add_piece = wN;
      else if(submove_str[2] == 'B') add_piece = wB;
      else if(submove_str[2] == 'R') add_piece = wR;
      else if(submove_str[2] == 'Q') add_piece = wQ;
      else if(submove_str[2] == 'K') add_piece = wK;
      else if(submove_str[2] == 'p') add_piece = bP;
      else if(submove_str[2] == 'n') add_piece = bN;
      else if(submove_str[2] == 'b') add_piece = bB;
      else if(submove_str[2] == 'r') add_piece = bR;
      else if(submove_str[2] == 'q') add_piece = bQ;
      else if(submove_str[2] == 'k') add_piece = bK;

      ASSERT(IS_SQUARE_ON_BOARD(add_square));
      ASSERT(IS_PIECE_VALID(add_piece));

      move.add_move(Move::create_add(add_square, add_piece));
    }

    return move;
  }else{
    Move move;
    move.valid = false;
    return move;
  }
}

bool Move::is_move(){
  if(this->move_count > 0){
    return (this->move[0] >> 18) & 0x1;
  }

  return false;
}

bool Move::is_add(){
  if(this->move_count > 0){
    return ~((this->move[0] >> 18) & 0x1);
  }

  return false;
}

bool Move::is_capture(){
  if(this->move_count > 0) return false;
  return this->get_captured() != EMPTY;
}

bool Move::is_valid(){
  return this->valid;
}

void Move::set_valid(bool valid){
  this->valid = valid;
}

void Move::add_move(int move){
  ASSERT(this->is_add() || this->move_count == 0);
  this->move[this->move_count++] = move;
}

int Move::get_move(int i){
  return this->move[i];
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

void Move::set_score(int score){
  this->score = score;
}

int Move::get_move_size(){
  return this->move_count;
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
