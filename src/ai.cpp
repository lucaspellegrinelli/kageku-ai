#include "ai.h"

void AI::search_position(Board *board, SearchInfo *info){
  Move best_move = Move::unvalid_move();
  int best_score = -INFINITE;
  AI::clear_for_search(board, info);

  for(int current_depth = 1; current_depth <= info->depth; current_depth++){
    best_score = AI::alpha_beta(-INFINITE, INFINITE, current_depth, board, info, true);
    int pv_moves = board->get_pv_line(current_depth);
    best_move = board->pv_array[0];
    std::cout << "Depth: " << current_depth << "  Score: " << best_score << "  Move: " << best_move.get_repr() << "  Nodes: " << info->nodes;
    std::cout << "  PV Line: ";
    for(int i = 0; i < pv_moves; i++){
      std::cout << board->get_from_pv_array(i).get_repr() << " ";
    }
    std::cout << std::endl;
    std::cout << "Ordering: " << (info->fail_high_first / info->fail_high) << std::endl;
  }
}

int AI::evaluate_board(Board *board){
  int material = board->material[WHITE] - board->material[BLACK];
  int mana = board->mana[WHITE] - board->mana[BLACK];

  int score = material + mana;

  if(board->side_to_move == WHITE) return score;
  else return -score;
}

void AI::check_up(){

}

void AI::clear_for_search(Board *board, SearchInfo *info){
  for(int i = 0; i < 13; i++){
    for(int j = 0; j < BOARD_SQ_NUM; j++){
      board->search_history[i][j] = 0;
    }
  }

  for(int i = 0; i < 2; i++){
    for(int j = 0; j < MAX_DEPTH; j++){
      board->search_killers[i][j] = 0;
    }
  }

  board->calculated_moves_table.clear();
  board->ply = 0;

  info->start_time = 0;
  info->stopped = 0;
  info->nodes = 0;
  info->fail_high = 0;
  info->fail_high_first = 0;
}

int AI::alpha_beta(int alpha, int beta, int depth, Board *board, SearchInfo *info, bool do_null){
  ASSERT(board->check_board());

  if(depth == 0){
    info->nodes++;
    return AI::evaluate_board(board);
  }

  info->nodes++;

  if(board->is_repetition() || board->fifty_move_counter >= 100){
    return 0;
  }

  if(board->ply >= MAX_DEPTH){
    return AI::evaluate_board(board);
  }

  MoveList move_list = board->generate_all_moves();

  int old_alpha = alpha;
  int legal_move_count = 0;
  Move best_move = Move::unvalid_move();
  int score = -INFINITE;

  for(int i = 0; i < move_list.count; i++){
    bool legal_move = board->make_move(move_list.moves[i]);
    if(!legal_move) continue;

    legal_move_count++;
    score = -AI::alpha_beta(-beta, -alpha, depth - 1, board, info, true);
    board->take_move();

    if(score > alpha){
      if(score >= beta){
        if(legal_move_count == 1){
          info->fail_high_first += 1;
        }
        info->fail_high += 1;
        return beta;
      }
      alpha = score;
      best_move = move_list.moves[i];
    }
  }

  if(legal_move_count == 0){
    int side = board->side_to_move;
    int opp_side = side == 0 ? 1 : 0;
    if(board->is_square_attacked(board->king_square[side], opp_side)){
      return -MATE + board->ply;
    }else return 0;
  }

  if(alpha != old_alpha){
    board->add_move_to_hash_table(best_move);
  }

  return alpha;
}

int AI::quiescense(int alpha, int beta, int depth, Board *board, SearchInfo *info){
  return 0;
}
