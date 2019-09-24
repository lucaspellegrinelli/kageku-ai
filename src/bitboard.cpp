#include "bitboard.h"

Bitboard::Bitboard(){
  this->bitboard = 0ULL;
}

int Bitboard::count(){
  U64 bb = this->bitboard;
  int r;
  for(r = 0; bb; r++, bb &= bb - 1);
  return r;
}

int Bitboard::pop(){
  U64 b = this->bitboard ^ (this->bitboard - 1);
  unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
  this->bitboard &= (this->bitboard - 1);
  return BIT_TABLE[(fold * 0x783a9b23) >> 26];
}

void Bitboard::clear_bit(int bit){
    this->bitboard &= ~(1ULL << bit);
}

void Bitboard::set_bit(int bit){
  this->bitboard |= (1ULL << bit);
}

void Bitboard::reset(){
  this->bitboard = 0ULL;
}

void Bitboard::print(){
  std::cout << std::endl;
  for(int rank = RANK_8; rank >= RANK_1; rank--){
    for(int file = FILE_A; file <= FILE_H; file++){
      int c_square = FILE_RANK_TO_SQUARE(file, rank);
      int c_square_64_board = square_from_120_board_to_64_board[c_square];

      U64 pos_to_check = 1ULL << c_square_64_board;
      if(pos_to_check & this->bitboard){
        std::cout << "X";
      }else{
        std::cout << "-";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}
