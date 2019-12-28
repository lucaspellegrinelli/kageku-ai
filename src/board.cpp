#include "board.h"

Board::Board(){
  this->initialize_piece_keys();
  this->initialize_side_key();
  this->initialize_mvvlva_scores();

  char starting_fen[] = "k1r5/ppp5/1p6/4n1PP/1p6/6P1/5PPP/5R1K w - 0 1";
  this->set_fen(starting_fen);

  this->update_lists_material();
}

MoveList Board::generate_all_moves(bool only_captures){
  ASSERT(this->check_board());

  MoveList list;

  int side = this->side_to_move;
  int opp_side = (side == WHITE ? BLACK : WHITE);

  const int side_pieces[2][6] = {
    {wP, wN, wB, wR, wQ, wK},
    {bP, bN, bB, bR, bQ, bK}
  };

  int forward_dir = (side == WHITE ? 1 : -1);
  int pawn_code = side_pieces[side][0];

  // Pawn move list
  for(int pawn_i = 0; pawn_i < this->piece_count[pawn_code]; pawn_i++){
    int sq = this->piece_list[pawn_code][pawn_i];
    ASSERT(IS_SQUARE_ON_BOARD(sq));

    int forward_sq = sq + 10 * forward_dir;
    if(this->pieces[forward_sq] == EMPTY && !only_captures){ // If there's no piece in front of the pawn
      ASSERT(IS_SQUARE_ON_BOARD(sq));
      ASSERT(IS_SQUARE_ON_BOARD(forward_sq));

      Move move(Move::create_move(sq, forward_sq));

      if((SQUARE_RANK[move.get_to()] == RANK_1 && side == BLACK) || (SQUARE_RANK[move.get_to()] == RANK_8 && side == WHITE)){
        move.set_score(990000);
      }else if(this->search_killers[0][this->ply].is_equal(move)){
        move.set_score(900000);
      }else if(this->search_killers[1][this->ply].is_equal(move)){
        move.set_score(800000);
      }else{
        move.set_score(this->search_history[this->pieces[sq]][forward_sq]);
      }

      if(list.count == 255){
        this->print();
        std::cout << "Number of moves:" << list.count << std::endl;
        list.print();
      }

      list.add_quiet_move(move);
    }

    int diag_sq[2] = {sq + 9 * forward_dir, sq + 11 * forward_dir};
    for(int diag : diag_sq){
      if(IS_SQUARE_ON_BOARD(diag) && PIECE_COLOR[this->pieces[diag]] == opp_side){ // If can eat diag
        ASSERT(IS_SQUARE_ON_BOARD(sq));
        ASSERT(IS_SQUARE_ON_BOARD(diag));
        ASSERT(IS_PIECE_VALID_OR_EMPTY(this->pieces[diag]));

        int move_i = Move::create_move(sq, diag, this->pieces[diag]);
        Move move(move_i);
        move.set_score(this->mvvlva_scores[move.get_captured()][this->pieces[sq]] + 1000000);
        list.add_capture_move(move);
      }
    }
  }

  // Other pieces
  for(int piece : side_pieces[side]){
    ASSERT(IS_PIECE_VALID(piece));

    for(int piece_i = 0; piece_i < this->piece_count[piece]; piece_i++){
      int sq = this->piece_list[piece][piece_i];
      ASSERT(IS_SQUARE_ON_BOARD(sq));

      for(int dir_i = 0; dir_i < PIECE_DIR_COUNT[piece]; dir_i++){
        int dir = PIECES_DIR[piece][dir_i];
        int t_sq = sq + dir;

        while(IS_SQUARE_ON_BOARD(t_sq)){
          if(this->pieces[t_sq] != EMPTY){
            if(PIECE_COLOR[this->pieces[t_sq]] == opp_side){
              int move_i = Move::create_move(sq, t_sq, this->pieces[t_sq]);
              Move move(move_i);
              move.set_score(this->mvvlva_scores[move.get_captured()][this->pieces[sq]] + 1000000);
              list.add_capture_move(move);
            }
          }else if(!only_captures){
            Move move(Move::create_move(sq, t_sq));

            if(this->search_killers[0][this->ply].is_equal(move)){
              move.set_score(900000);
            }else if(this->search_killers[1][this->ply].is_equal(move)){
              move.set_score(800000);
            }else{
              move.set_score(this->search_history[this->pieces[sq]][t_sq]);
            }

            list.add_quiet_move(move);
          }

          if(IS_SLIDER_PIECE[piece]){
            if(this->pieces[t_sq] != EMPTY) break;
            t_sq += dir;
          }else{
            break;
          }
        }
      }
    }
  }

  if(!only_captures){
    // Piece adds
    int addable_positions[MAX_POSITION_MOVES];
    int unchecked_positions[BOARD_SQ_NUM];
    bool position_status[BOARD_SQ_NUM];

    for(int i = 0; i < BOARD_SQ_NUM; position_status[i++] = false);

    unchecked_positions[0] = this->king_square[side];

    int add_count = 0;
    int unchecked_count = 1;

    for(int i = 0; i < unchecked_count; i++){
      int pos = unchecked_positions[i];
      int adj[4] = {pos + 10, pos - 10, pos + 1, pos - 1};

      for(int adj_pos : adj){
        if(IS_SQUARE_ON_BOARD(adj_pos)){
          int piece = this->pieces[adj_pos];

          // Cant add to the opponent side
          if((SQUARE_RANK[adj_pos] >= RANK_5) == (side == WHITE)) continue;

          if(!position_status[adj_pos]){
            if(piece == EMPTY) addable_positions[add_count++] = adj_pos;
            else if(PIECE_COLOR[piece] == side) unchecked_positions[unchecked_count++] = adj_pos;
            position_status[adj_pos] = true;
          }
        }
      }
    }

    // Each line is a move, each column a position on the board a piece is added
    int all_adds[MAX_POSITION_MOVES][MAX_ADD_PER_MOVE];
    int all_adds_size = 0;
    int each_add_size[MAX_POSITION_MOVES];
    for(int i = 0; i < MAX_POSITION_MOVES; each_add_size[i++] = 0);

    for(int add_qtd = 1; add_qtd <= MAX_ADD_PER_MOVE; add_qtd++){
      std::vector<bool> v = std::vector<bool>(add_count);
    	std::fill(v.begin(), v.begin() + add_qtd, true);
    	std::vector<std::vector<int>> positions_to_add;

    	do {
    		std::vector<int> c_perm;
    		for (int i = 0; i < add_count; ++i) {
    			if (v[i]) c_perm.push_back(i);
    		}

        std::vector<int> end_c_perm;
        for(size_t i = 0; i < c_perm.size(); i++){
          end_c_perm.push_back(addable_positions[c_perm[i]]);
        }
    		positions_to_add.push_back(end_c_perm);
    	}while (std::prev_permutation(v.begin(), v.end()));

      // Each line in 'positions_to_add' is a list of positions of size 'add_qtd'
      // where the code will add some piece

      const int option_count = 5; // Pawn, Knight, Bishop, Rook and Queen
      std::vector<std::vector<int>> all_option_comb;
      std::vector<int> option_comb(add_qtd + 1, 0);
      while(true){
        for(int i = 0; i < add_qtd; i++){
          if(option_comb[i] >= option_count){
            option_comb[i + 1]++;
            for(int k = i; k >= 0; option_comb[k--] = option_comb[i + 1]);
          }
        }

        if(option_comb[add_qtd] > 0) break;

        int option_price = 0;
        int option_qtd[PIECE_TYPES];
        for(int i = 0; i < PIECE_TYPES; i++){
          option_qtd[i] = this->piece_count[i];
        }

        std::vector<int> end_option_comb;
        for(size_t i = 0; i < option_comb.size() - 1; i++){
          int piece_add = side_pieces[this->side_to_move][option_comb[i]];
          ASSERT(IS_PIECE_VALID(piece_add));
          option_price += PIECE_COST[piece_add];
          option_qtd[piece_add]++;
          end_option_comb.push_back(piece_add);
        }

        bool is_qtd_ok = true;
        for(int i = 1; i < PIECE_TYPES; i++){
          if(option_qtd[i] > PIECE_MAX[i]){
            is_qtd_ok = false;
            break;
          }
        }

        if(option_price <= this->mana[side] && is_qtd_ok){
          all_option_comb.push_back(end_option_comb);
        }

        option_comb[0]++;
      }

      // Each line in 'all_option_comb' is a list of pieces of size 'add_qtd'
      // to be added in one of the positions

      for(std::vector<int> c_pos_to_add : positions_to_add){
        for(std::vector<int> c_options : all_option_comb){
          // Permutating the options list so that each piece is in each position
          do{
            each_add_size[all_adds_size] = c_pos_to_add.size();
            for(size_t i = 0; i < c_pos_to_add.size(); i++){
              all_adds[all_adds_size][i] = Move::create_add(c_pos_to_add[i], c_options[i]);
            }
            all_adds_size++;
          }while(std::next_permutation(c_options.begin(), c_options.end()));
        }
      }
    }

    for(int i = 0; i < all_adds_size; i++){
      Move move;
      for(int j = 0; j < each_add_size[i]; j++){
        move.set_score(800000);
        move.add_move(all_adds[i][j]);
      }

      list.add_new_piece_move(move);
    }
  }

  return list;
}

void Board::clear_piece(const int square){
  ASSERT(IS_SQUARE_ON_BOARD(square));
  int piece = this->pieces[square];
  ASSERT(IS_PIECE_VALID(piece));

  int color = PIECE_COLOR[piece];
  int piece_list_id = -1;

  HASH_PIECE(piece, square);

  this->pieces[square] = EMPTY;
  this->material[color] -= PIECE_VAL[piece];

  if(NON_PAWN_PIECES[piece]){
    this->not_pawn_piece_count[color]--;
    if(MAJOR_PIECES[piece]) this->major_piece_count[color]--;
    else this->minor_piece_count[color]--;
  }else{
    this->pawns[color].clear_bit(square_from_120_board_to_64_board[square]);
    this->pawns[BOTH].clear_bit(square_from_120_board_to_64_board[square]);
  }

  for(int i = 0; i < this->piece_count[piece]; i++){
    if(this->piece_list[piece][i] == square){
      piece_list_id = i;
      break;
    }
  }

  ASSERT(piece_list_id != -1);

  this->piece_count[piece]--;
  this->piece_list[piece][piece_list_id] = this->piece_list[piece][this->piece_count[piece]];
}

void Board::add_piece(const int piece, const int square){
  ASSERT(IS_PIECE_VALID(piece));
  ASSERT(IS_SQUARE_ON_BOARD(square));

  int color = PIECE_COLOR[piece];

  HASH_PIECE(piece, square);
  this->pieces[square] = piece;

  if(NON_PAWN_PIECES[piece]){
    this->not_pawn_piece_count[color]++;
    if(MAJOR_PIECES[piece]) this->major_piece_count[color]++;
    else this->minor_piece_count[color]++;
  }else{
    this->pawns[color].set_bit(square_from_120_board_to_64_board[square]);
    this->pawns[BOTH].set_bit(square_from_120_board_to_64_board[square]);
  }

  this->material[color] += PIECE_VAL[piece];
  this->piece_list[piece][this->piece_count[piece]++] = square;
}

void Board::move_piece(const int from, const int to){
  ASSERT(IS_SQUARE_ON_BOARD(from));
  ASSERT(IS_SQUARE_ON_BOARD(to));

  int piece = this->pieces[from];
  int color = PIECE_COLOR[piece];

  #ifdef DEBUG
  bool piece_found_in_list = false;
  #endif

  HASH_PIECE(piece, from);
  this->pieces[from] = EMPTY;

  HASH_PIECE(piece, to);
  this->pieces[to] = piece;

  if(!NON_PAWN_PIECES[piece]){
    this->pawns[color].clear_bit(square_from_120_board_to_64_board[from]);
    this->pawns[BOTH].clear_bit(square_from_120_board_to_64_board[from]);
    this->pawns[color].set_bit(square_from_120_board_to_64_board[to]);
    this->pawns[BOTH].set_bit(square_from_120_board_to_64_board[to]);

    if(color == WHITE){
      if(SQUARE_RANK[from] <= RANK_4 && SQUARE_RANK[to] > RANK_4){
        this->mana[WHITE]++;
      }else if(SQUARE_RANK[to] <= RANK_4 && SQUARE_RANK[from] > RANK_4){
        this->mana[WHITE]--;
      }
    }else{
      if(SQUARE_RANK[from] > RANK_4 && SQUARE_RANK[to] <= RANK_4){
        this->mana[BLACK]++;
      }else if(SQUARE_RANK[to] > RANK_4 && SQUARE_RANK[from] <= RANK_4){
        this->mana[BLACK]--;
      }
    }
  }

  for(int i = 0; i < this->piece_count[piece]; i++){
    if(this->piece_list[piece][i] == from){
      this->piece_list[piece][i] = to;
      #ifdef DEBUG
      piece_found_in_list = true;
      #endif
      break;
    }
  }

  ASSERT(piece_found_in_list);
}

bool Board::make_move(Move move){
  ASSERT(this->check_board());

  if(move.is_move()){
    int from = move.get_from();
    int to = move.get_to();
    int side = this->side_to_move;

    ASSERT(IS_SQUARE_ON_BOARD(from));
    ASSERT(IS_SQUARE_ON_BOARD(to));
    ASSERT(IS_SIDE_VALID(side));
    ASSERT(IS_PIECE_VALID(this->pieces[from]));

    this->history[this->history_ply].position_key = this->position_key;
    this->history[this->history_ply].move = move;
    this->history[this->history_ply].fifty_move_counter = this->fifty_move_counter;

    int captured = move.get_captured();
    this->fifty_move_counter++;

    if(captured != EMPTY){
      ASSERT(IS_PIECE_VALID(captured));
      this->clear_piece(to);
      this->fifty_move_counter = 0;
    }

    this->history_ply++;
    this->ply++;

    this->move_piece(from, to);

    if(IS_PIECE_KING[this->pieces[to]]){
      this->king_square[this->side_to_move] = to;
    }

    this->side_to_move = this->side_to_move == 0 ? 1 : 0;
    HASH_SIDE;

    if(this->is_square_attacked(this->king_square[side], this->side_to_move)){
      this->take_move();
      return false;
    }

    ASSERT(this->check_board());

    return true;
  }else{
    int side = this->side_to_move;

    this->history[this->history_ply].position_key = this->position_key;
    this->history[this->history_ply].move = move;
    this->history[this->history_ply].fifty_move_counter = this->fifty_move_counter;

    this->history_ply++;
    this->ply++;

    this->fifty_move_counter++;

    // if(move.get_repr() == "e1Pg4Pf3P"){
    //   std::cout << "\n\n\n\n\n\n";
    //   std::cout << move.get_repr() << std::endl;
    //   this->print();
    // }

    for(int i = 0; i < move.get_move_size(); i++){
      int add_square = move.get_add_square(i);
      int add_piece = move.get_add_piece(i);
      int side = this->side_to_move;

      ASSERT(IS_PIECE_VALID(add_piece));
      ASSERT(IS_SQUARE_ON_BOARD(add_square));
      ASSERT(IS_SIDE_VALID(side));
      ASSERT(this->pieces[add_square] == EMPTY);
      this->add_piece(add_piece, add_square);
    }

    // if(move.get_repr() == "e1Pg4Pf3P"){
    //   this->print();
    //   std::cout << "\n\n\n\n\n\n";
    // }

    this->side_to_move = this->side_to_move == 0 ? 1 : 0;
    HASH_SIDE;

    if(this->is_square_attacked(this->king_square[side], this->side_to_move)){
      this->take_move();
      return false;
    }

    ASSERT(this->check_board());
    return true;
  }
}

void Board::take_move(){
  ASSERT(this->check_board());

  this->history_ply--;
  this->ply--;

  Move move = this->history[this->history_ply].move;

  if(move.is_move()){
    int from = move.get_from();
    int to = move.get_to();

    ASSERT(IS_SQUARE_ON_BOARD(from));
    ASSERT(IS_SQUARE_ON_BOARD(to));

    this->fifty_move_counter = this->history[this->history_ply].fifty_move_counter;
    this->side_to_move = this->side_to_move == 0 ? 1 : 0;
    HASH_SIDE;

    this->move_piece(to, from);

    if(IS_PIECE_KING[this->pieces[from]]){
      this->king_square[this->side_to_move] = from;
    }

    int captured = move.get_captured();
    if(captured != EMPTY){
      ASSERT(IS_PIECE_VALID(captured));
      this->add_piece(captured, to);
    }
  }else{
    for(int i = 0; i < move.get_move_size(); i++){
      int add_square = move.get_add_square(i);
      int add_piece = move.get_add_piece(i);

      ASSERT(IS_PIECE_VALID(add_piece));
      ASSERT(IS_SQUARE_ON_BOARD(add_square));
      ASSERT(this->pieces[add_square] != EMPTY);

      this->clear_piece(add_square);
    }

    this->fifty_move_counter = this->history[this->history_ply].fifty_move_counter;
    this->side_to_move = this->side_to_move == 0 ? 1 : 0;
    HASH_SIDE;
  }

  ASSERT(this->check_board());
}

bool Board::is_repetition(){
  for(int i = 0; i < this->history_ply; i++){
    if(this->position_key == this->history[i].position_key){
      return true;
    }
  }

  return false;
}

void Board::print_square_attacks(int side){
  ASSERT(IS_SIDE_VALID(side));
  ASSERT(this->check_board());

  for(int i = 7; i >= 0; i--){
    for(int j = 0; j < 8; j++){
      int index = i * 8 + j;
      int sq_120_index = square_from_64_board_to_120_board[index];
      bool attacked = this->is_square_attacked(sq_120_index, side);
      std::cout << attacked << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

bool Board::is_square_attacked(int sq, int side){
  ASSERT(IS_SQUARE_ON_BOARD(sq));
  ASSERT(IS_SIDE_VALID(side));
  ASSERT(this->check_board());

  // Pawns
  if(side == WHITE){
    if(this->pieces[sq - 11] == wP || this->pieces[sq - 9] == wP) return true;
  }else if(side == BLACK){
    if(this->pieces[sq + 11] == bP || this->pieces[sq + 9] == bP) return true;
  }

  // Knights
  for(int dir_i = 0; dir_i < 8; dir_i++){
    int piece = this->pieces[sq + KNIGHT_DIR[dir_i]];
    if(piece != OFFBOARD && IS_PIECE_KNIGHT[piece] && PIECE_COLOR[piece] == side) return true;
  }

  // King
  for(int dir_i = 0; dir_i < 8; dir_i++){
    int piece = this->pieces[sq + KING_DIR[dir_i]];
    if(piece != OFFBOARD && IS_PIECE_KING[piece] && PIECE_COLOR[piece] == side) return true;
  }

  // Rook and Queen
  for(int dir_i = 0; dir_i < 4; dir_i++){
    int dir = ROOK_DIR[dir_i];
    int t_sq = sq + dir;
    int piece = this->pieces[t_sq];

    while(piece != OFFBOARD){
      if(piece != EMPTY){
        if(IS_PIECE_ROOK_QUEEN[piece] && PIECE_COLOR[piece] == side) return true;
        break;
      }
      t_sq += dir;
      piece = this->pieces[t_sq];
    }
  }

  // Bishop and Queen
  for(int dir_i = 0; dir_i < 4; dir_i++){
    int dir = BISHOP_DIR[dir_i];
    int t_sq = sq + dir;
    int piece = this->pieces[t_sq];

    while(piece != OFFBOARD){
      if(piece != EMPTY){
        if(IS_PIECE_BISHOP_QUEEN[piece] && PIECE_COLOR[piece] == side) return true;
        break;
      }
      t_sq += dir;
      piece = this->pieces[t_sq];
    }
  }

  return false;
}

bool Board::is_promotion(int side){
  int start_pos = side == WHITE ? 91 : 21;
  int pawn_side = side == WHITE ? wP : bP;

  for(int i = 0; i < 8; i++){
    if(this->pieces[start_pos + i] == pawn_side){
      return true;
    }
  }

  return false;
}

void Board::update_lists_material(){
  bool has_close_pawn[2] = {false, false};

  this->mana[WHITE] = 0;
  this->mana[BLACK] = 0;

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

        if(SQUARE_RANK[i] > RANK_4) this->mana[WHITE]++;
        if(SQUARE_RANK[i] <= RANK_4) has_close_pawn[WHITE] = true;
      }else if(piece == bP){
        this->pawns[BLACK].set_bit(square_from_120_board_to_64_board[sq]);
        this->pawns[BOTH].set_bit(square_from_120_board_to_64_board[sq]);

        if(SQUARE_RANK[i] <= RANK_4) this->mana[BLACK]++;
        if(SQUARE_RANK[i] > RANK_4) has_close_pawn[BLACK] = true;
      }
    }
  }

  if(has_close_pawn[WHITE]) this->mana[WHITE]++;
  if(has_close_pawn[BLACK]) this->mana[BLACK]++;
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
      if(this->pieces[sq120] != t_piece){
        std::cout << this->pieces[sq120] << " != " << t_piece << " in " << sq120 << std::endl;
      }
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
  std::ostream boardcout(std::cout.rdbuf());

  for(int r = RANK_8; r >= RANK_1; r--){
    boardcout << r + 1 << " ";
    for(int f = FILE_A; f <= FILE_H; f++){
      int sq = FILE_RANK_TO_SQUARE(f, r);
      int piece = this->pieces[sq];
      boardcout << std::setw(2) << PIECE_CHARS[piece];
    }

    boardcout << std::endl;
  }

  boardcout << std::endl << "  ";
  for(int f = FILE_A; f <= FILE_H; f++){
    boardcout << std::setw(2) << (char)('a' + f);
  }
  boardcout << std::endl << std::endl;

  boardcout << "Side: " << COLOR_CHARS[this->side_to_move] << std::endl;
  boardcout << "Position Key: " << std::hex << std::uppercase << this->position_key << std::endl;
  boardcout << "Side mana: " << this->mana[WHITE] << std::endl;
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
    this->material[i] = 0;
  }

  for(int i = 0; i < COLOR_COUNT + 1; i++){
    this->pawns[i].reset();
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

void Board::add_move_to_hash_table(Move move){
  int hash_index = this->position_key % PV_TABLE_ENTRY_COUNT;

  std::pair<U64, Move> entry = std::make_pair(this->position_key, move);
  std::pair<int, std::pair<U64, Move>> item = std::make_pair(hash_index, entry);

  if(this->calculated_moves_table.find(hash_index) != this->calculated_moves_table.end()){
    this->calculated_moves_table[hash_index] = entry;
  }else{
    this->calculated_moves_table.insert(item);
  }
}

Move Board::get_move_from_hash_table(){
  int hash_index = this->position_key % PV_TABLE_ENTRY_COUNT;

  std::pair<U64, Move> entry = this->calculated_moves_table[hash_index];

  if(entry.first == this->position_key){
    return entry.second;
  }

  return Move::unvalid_move();
}

int Board::get_pv_line(int depth){
  ASSERT(depth <= MAX_DEPTH);

  Move move = this->get_move_from_hash_table();
  int count = 0;

  while(move.is_valid() && count < depth){
    ASSERT(count < MAX_DEPTH);

    if(this->generate_all_moves().is_move_in_list(move)){
      this->make_move(move);
      this->pv_array[count++] = move;
    }else{
      break;
    }

    move = this->get_move_from_hash_table();
  }

  while(this->ply > 0){
    this->take_move();
  }

  return count;
}

Move Board::get_from_pv_array(int i){
  return this->pv_array[i];
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

void Board::initialize_mvvlva_scores(){
  for(int i = wP; i <= bK; i++){
    for(int j = wP; j <= bK; j++){
      this->mvvlva_scores[i][j] = VICTIM_SCORE[j] + 6 - (VICTIM_SCORE[i] / 100);
    }
  }
}

Board::~Board(){
  // for (auto& it : this->calculated_moves_table) {
  //   delete it.second;
  // }
}
