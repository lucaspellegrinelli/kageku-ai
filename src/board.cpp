#include "board.h"

Board::Board(){
  this->initialize_piece_keys();
  this->initialize_side_key();

  char starting_fen[] = "k1r5/ppp5/8/8/8/8/5PPP/5R1K w - 0 1";
  this->set_fen(starting_fen);
}

void Board::print(){
  for(int r = RANK_8; r >= RANK_1; r--){
    std::cout << r + 1 << " ";
    for(int f = FILE_A; f <= FILE_H; f++){
      int sq = FILE_RANK_TO_SQUARE(f, r);
      int piece = this->pieces[sq];
      std::cout << std::setw(2) << PIECE_CHARS[piece];
    }

    std::cout << std::endl;
  }

  std::cout << std::endl << "  ";
  for(int f = FILE_A; f <= FILE_H; f++){
    std::cout << std::setw(2) << (char)('a' + f);
  }
  std::cout << std::endl << std::endl;

  std::cout << "Side: " << COLOR_CHARS[this->side_to_move] << std::endl;
  std::cout << "Position Key: " << std::hex << std::uppercase << this->position_key << std::endl;
}

void Board::set_fen(char *fen){
  ASSERT(fen != nullptr);

  this->reset();

  int piece = 0;
  int rank = RANK_8;
  int file = FILE_A;
  int count = 0;

  while(rank >= RANK_1 && *fen){
    count = 1;
    switch(*fen){
      case 'p': piece = bP; break;
      case 'n': piece = bN; break;
      case 'b': piece = bB; break;
      case 'r': piece = bR; break;
      case 'q': piece = bQ; break;
      case 'k': piece = bK; break;
      case 'P': piece = wP; break;
      case 'N': piece = wN; break;
      case 'B': piece = wB; break;
      case 'R': piece = wR; break;
      case 'Q': piece = wQ; break;
      case 'K': piece = wK; break;

      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
        piece = EMPTY;
        count = *fen - '0';
        break;

      case '/':
      case ' ':
        rank--;
        file = FILE_A;
        fen++;
        continue;

      default:
        std::cout << "FEN error" << std::endl;
        exit(1);
    }

    for(int i = 0; i < count; i++){
      int sq64 = rank * 8 + file;
      int sq120 = square_from_64_board_to_120_board[sq64];
      if(piece != EMPTY) this->pieces[sq120] = piece;
      file++;
    }

    fen++;
  }

  ASSERT(*fen == 'w' || *fen == 'b');

  this->side_to_move = (*fen == 'w') ? WHITE : BLACK;
  this->position_key = this->generate_position_key();
}

void Board::reset(){
  for(int i = 0; i < BOARD_SQ_NUM; i++){
    this->pieces[i] = OFFBOARD;
  }

  for(int i = 0; i < 64; i++){
    this->pieces[square_from_64_board_to_120_board[i]] = EMPTY;
  }

  for(int i = 0; i < COLOR_COUNT + 1; i++){
    this->not_pawn_piece_count[i] = 0;
    this->major_piece_count[i] = 0;
    this->minor_piece_count[i] = 0;
    this->pawns[i] = 0ULL;
  }

  for(int i = 0; i < PIECE_TYPES; i++){
    this->piece_count[i] = 0;
  }

  this->king_square[WHITE] = NO_SQUARE;
  this->king_square[BLACK] = NO_SQUARE;

  this->side_to_move = BOTH;
  this->fifty_move_counter = 0;

  this->ply = 0;
  this->history_ply = 0;

  this->position_key = 0ULL;
}

U64 Board::generate_position_key(){
  U64 final_key = 0;

  for(int sq = 0; sq < BOARD_SQ_NUM; sq++){
    int piece = this->pieces[sq];

    if(piece != NO_SQUARE && piece != EMPTY && piece != OFFBOARD){
      ASSERT(piece >= wP && piece <= bK);
      final_key ^= this->piece_keys[piece][sq];
    }
  }

  if(this->side_to_move == WHITE){
    final_key ^= this->side_key;
  }

  return final_key;
}

void Board::initialize_side_key(){
  U64 r = (U64)rand() | ((U64)rand() << 15) | ((U64)rand() << 30) | ((U64)rand() << 45) | (((U64)rand() & 0xf) << 60);
  this->side_key = r;
}

void Board::initialize_piece_keys(){
  for(int i = 0; i < PIECE_TYPES; i++){
    for(int j = 0; j < BOARD_SQ_NUM; j++){
      U64 r = (U64)rand() | ((U64)rand() << 15) | ((U64)rand() << 30) | ((U64)rand() << 45) | (((U64)rand() & 0xf) << 60);
      this->piece_keys[i][j] = r;
    }
  }
}
