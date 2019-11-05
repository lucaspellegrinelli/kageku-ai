#include "ai.h"

void AI::search_position(Board *board, SearchInfo *info){
  Move best_move = Move::unvalid_move();
  int best_score = -INFINITE;
  AI::clear_for_search(board, info);

  for(int current_depth = 1; current_depth < info->depth; current_depth++){
    best_score = AI::alpha_beta(-INFINITE, INFINITE, current_depth, board, info, true);
    int pv_moves = board->get_pv_line(current_depth);
    best_move = board->pv_array[0];
    std::cout << "Depth: " << current_depth << "  Score: " << best_score << "  Move: " << best_move.get_repr() << "  Nodes: " << info->nodes << std::endl;

    int pv_moves = b->get_pv_line(current_depth);
    std::cout << "PV Line: ";
    for(int i = 0; i < pv_moves; i++){
      std::cout << b->get_from_pv_array(i).get_repr() << " ";
    }
    std::cout << std::endl;
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
}

int AI::alpha_beta(int alpha, int beta, int depth, Board *board, SearchInfo *info, bool do_null){
  return 0;
}

int AI::quiescense(int alpha, int beta, int depth, Board *board, SearchInfo *info){
  return 0;
}
