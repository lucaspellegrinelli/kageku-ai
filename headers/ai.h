#ifndef AI_H
#define AI_H

#include <chrono>

#include "board.h"
#include "move.h"
#include "move_list.h"

#include "defs.h"
#include "macros.h"

using namespace std::chrono;

class SearchInfo{
public:
  long start_time;
  long stop_time;
  int depth;
  int depth_set;
  int time_set;
  int moves_to_go;
  int infinite;

  long nodes;

  int quit;
  int stopped;

  float fail_high;
  float fail_high_first;

  SearchInfo(){ }
};

class AI{
public:
  static int evaluate_board(Board *board);
  static Move search_position(Board *board, SearchInfo *info);
  static void check_up();
  static void clear_for_search(Board *board, SearchInfo *info);

  static int alpha_beta(int alpha, int beta, int depth, Board *board, SearchInfo *info, bool do_null);
  static int quiescense(int alpha, int beta, Board *board, SearchInfo *info);
};

#endif
