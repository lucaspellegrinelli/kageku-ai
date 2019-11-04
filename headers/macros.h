#ifndef MACROS_H
#define MACROS_H

#include "stdio.h"
#include "defs.h"

// Define an assert
#define DEBUG
#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed", #n); \
printf("\nOn %s ", __DATE__); \
printf("\nAt %s ", __TIME__); \
printf("\nIn File %s ", __FILE__); \
printf("\nAt Line %d\n", __LINE__); \
exit(1); \
}
#endif

// Converts a file and a rank to a 120-board square
#define FILE_RANK_TO_SQUARE(f, r) ((21 + (f)) + ((r) * 10))

// Validates some stuff
#define IS_SQUARE_OFFBOARD(sq) (SQUARE_FILE[(sq)] == OFFBOARD)
#define IS_SQUARE_ON_BOARD(sq) (SQUARE_FILE[(sq)] != OFFBOARD)
#define IS_SIDE_VALID(side) ((side) == WHITE || (side) == BLACK)
#define IS_FILE_RANK_VALID(fr) ((fr) >= 0 && fr <= 7)
#define IS_PIECE_REPR_VALID(piece) ((piece) == 'p' || (piece) == 'n' || (piece) == 'b' || (piece) == 'r' || (piece) == 'q' || (piece) == 'k' || (piece) == 'P' || (piece) == 'N' || (piece) == 'B' || (piece) == 'R' || (piece) == 'Q' || (piece) == 'K')
#define IS_PIECE_VALID(piece) ((piece) >= wP && (piece) <= bK)
#define IS_PIECE_VALID_OR_EMPTY(piece) ((piece) >= EMPTY && (piece) <= bK)

// Converts piece to index
#define PIECE_SIDE_INDEX(piece) ((PIECE_COLOR[(piece)] == WHITE) ? ((piece) - wP) : ((piece) - bP))

#endif
