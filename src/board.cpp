#include "board.h"

Board::Board(){
  this->initialize_piece_keys();
  this->initialize_side_key();

  char starting_fen[] = "k1r5/ppp5/8/8/8/8/5PPP/5R1K w - 0 1";
  this->set_fen(starting_fen);

  this->update_lists_material();
}

void Board::update_lists_material(){
  for(int i = 0; i < BOARD_SQ_NUM; i++){
    int sq = i;
    int piece = this->pieces[i];
    if(piece != OFFBOARD && piece != EMPTY){
      int color = PIECE_COLOR[piece];

      if(NON_PAWN_PIECES[piece]) this->not_pawn_piece_count[color]++;
      if(MINOR_PIECES[piece]) this->minor_piece_count[color]++;
      if(MAJOR_PIECES[piece]) this->major_piece_count[color]++;

      this->material[color] += PIECE_VAL[piece];

      this->piece_list[piece][this->piece_count[piece]] = sq;
      this->piece_count[piece]++;

      if(piece == wK) this->king_square[WHITE] = sq;
      if(piece == bK) this->king_square[BLACK] = sq;

      if(piece == wP){
        this->pawns[WHITE].set_bit(square_from_120_board_to_64_board[sq]);
        this->pawns[BOTH].set_bit(square_from_120_board_to_64_board[sq]);
      }else if(piece == bP){
        this->pawns[BLACK].set_bit(square_from_120_board_to_64_board[sq]);
        this->pawns[BOTH].set_bit(square_from_120_board_to_64_board[sq]);
      }
    }
  }
}

bool Board::check_board(){
  int t_piece_count[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int t_non_pawn_piece[2] = {0, 0};
  int t_major_piece[2] = {0, 0};
  int t_minor_piece[2] = {0, 0};
  int t_material[2] = {0, 0};

  Bitboard t_pawns[3];
  t_pawns[WHITE] = this->pawns[WHITE];
  t_pawns[BLACK] = this->pawns[BLACK];
  t_pawns[BOTH] = this->pawns[BOTH];

  for(int t_piece = wP; t_piece <= bK; t_piece++){
    for(int t_count = 0; t_count < this->piece_count[t_piece]; t_count++){
      int sq120 = this->piece_list[t_piece][t_count];
      ASSERT(this->pieces[sq120] == t_piece);
    }
  }

  for(int sq64 = 0; sq64 < 64; sq64++){
    int sq120 = square_from_64_board_to_120_board[sq64];
    int t_piece = this->pieces[sq120];
    t_piece_count[t_piece]++;
    int color = PIECE_COLOR[t_piece];

    if(NON_PAWN_PIECES[t_piece]) t_non_pawn_piece[color]++;
    if(MINOR_PIECES[t_piece]) t_minor_piece[color]++;
    if(MAJOR_PIECES[t_piece]) t_major_piece[color]++;

    t_material[color] += PIECE_VAL[t_piece];
  }

  // Checks if the number of pieces is ok
  for(int t_piece = wP; t_piece <= bK; t_piece++){
    ASSERT(t_piece_count[t_piece] == this->piece_count[t_piece]);
  }

  // Checks if the pawn bitboard is ok
  ASSERT(t_pawns[WHITE].count() == this->pawns[WHITE].count());
  ASSERT(t_pawns[BLACK].count() == this->pawns[BLACK].count());
  ASSERT(t_pawns[BOTH].count() == this->pawns[BOTH].count());

  while(t_pawns[WHITE].bitboard){
    int sq64 = t_pawns[WHITE].pop();
    ASSERT(this->pieces[square_from_64_board_to_120_board[sq64]] == wP);
  }

  while(t_pawns[BLACK].bitboard){
    int sq64 = t_pawns[BLACK].pop();
    ASSERT(this->pieces[square_from_64_board_to_120_board[sq64]] == bP);
  }

  while(t_pawns[BOTH].bitboard){
    int sq64 = t_pawns[BOTH].pop();
    ASSERT(this->pieces[square_from_64_board_to_120_board[sq64]] == wP ||
           this->pieces[square_from_64_board_to_120_board[sq64]] == bP);
  }

  // Assert material, major pieces...
  ASSERT(t_material[WHITE] == this->material[WHITE]);
  ASSERT(t_material[BLACK] == this->material[BLACK]);
  ASSERT(t_non_pawn_piece[WHITE] == this->not_pawn_piece_count[WHITE]);
  ASSERT(t_non_pawn_piece[BLACK] == this->not_pawn_piece_count[BLACK]);
  ASSERT(t_major_piece[WHITE] == this->major_piece_count[WHITE]);
  ASSERT(t_major_piece[BLACK] == this->major_piece_count[BLACK]);
  ASSERT(t_minor_piece[WHITE] == this->minor_piece_count[WHITE]);
  ASSERT(t_minor_piece[BLACK] == this->minor_piece_count[BLACK]);

  // Assert key and side
  ASSERT(this->side_to_move == WHITE || this->side_to_move == BLACK);
  ASSERT(this->generate_position_key() == this->position_key);

  // Assert king pos
  ASSERT(this->pieces[this->king_square[WHITE]] == wK);
  ASSERT(this->pieces[this->king_square[BLACK]] == bK);

  return true;
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

  for(int i = 0; i < COLOR_COUNT; i++){
    this->not_pawn_piece_count[i] = 0;
    this->major_piece_count[i] = 0;
    this->minor_piece_count[i] = 0;
  }

  for(int i = 0; i < COLOR_COUNT + 1; i++){
    this->pawns[i].reset();
  }

  for(int i = 0; i < PIECE_TYPES; i++){
    this->piece_count[i] = 0;
  }

  this->king_square[WHITE] = NO_SQUARE;
  this->king_square[BLACK] = NO_SQUARE;

  this->material[WHITE] = 0;
  this->material[BLACK] = 0;

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
