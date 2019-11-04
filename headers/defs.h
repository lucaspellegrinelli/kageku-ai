#ifndef DEFS_H
#define DEFS_H

#define ENGINE_NAME "Kai"

// Number of squares in the board
#define BOARD_SQ_NUM 120

// Int with 64 bits (8x8 board = 64 squares!)
typedef unsigned long long U64;

// Maximum number of moves a game can have
#define MAX_GAME_MOVES 2048

// Maximum number of possible moves per position
#define MAX_POSITION_MOVES 256

#define NOMOVE 0

// Flags for colors (BOTH - Respresents pieces of either color)
#define COLOR_COUNT 2
enum{WHITE, BLACK, BOTH};
const char COLOR_CHARS[] = "wb-";

// Flags for each of the pieces
#define PIECE_TYPES 13
enum{EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK};
const char PIECE_CHARS[] = ".PNBRQKpnbrqk";

// Defines the direction each piece moves
const int EMPTY_DIR[8] = {0, 0, 0, 0, 0, 0, 0, 0};
const int PAWN_DIR[8] = {0, 0, 0, 0, 0, 0, 0, 0};
const int KNIGHT_DIR[8] = {-8, -19, -21, 12, 8, 19, 21, 12};
const int ROOK_DIR[8] = {-1, -10, 1, 10, 0, 0, 0, 0};
const int BISHOP_DIR[8] = {-9, -11, 11, 9, 0, 0, 0, 0};
const int QUEEN_DIR[8] = {-1, -10, 1, 10, -9, -11, 11, 9};
const int KING_DIR[8] = {-1, -10, 1, 10, -9, -11, 11, 9};
const int PIECE_DIR_COUNT[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8}; // How many directions each piece has
const int PIECES_DIR[13][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {-8, -19, -21, 12, 8, 19, 21, 12},
  {-9, -11, 11, 9, 0, 0, 0, 0},
  {-1, -10, 1, 10, 0, 0, 0, 0},
  {-1, -10, 1, 10, -9, -11, 11, 9},
  {-1, -10, 1, 10, -9, -11, 11, 9},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {-8, -19, -21, 12, 8, 19, 21, 12},
  {-9, -11, 11, 9, 0, 0, 0, 0},
  {-1, -10, 1, 10, 0, 0, 0, 0},
  {-1, -10, 1, 10, -9, -11, 11, 9},
  {-1, -10, 1, 10, -9, -11, 11, 9}
};

// Defines which are the major pieces, minor pieces ...
const int NON_PAWN_PIECES[] = {false, false, true, true, true, true, true, false, true, true, true, true, true};
const int MAJOR_PIECES[] = {false, false, false, false, true, true, true, false, false, false, true, true, true};
const int MINOR_PIECES[] = {false, false, true, true, false, false, false, false, true, true, false, false, false};
const int PIECE_COLOR[] = {BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};

// Define pieces identity
const int IS_PIECE_KNIGHT[13] = {false, false, true, false, false, false, false, false, true, false, false, false, false};
const int IS_PIECE_KING[13] = {false, false, false, false, false, false, true, false, false, false, false, false, true};
const int IS_PIECE_ROOK_QUEEN[13] = {false, false, false, false, true, true, false, false, false, false, true, true, false};
const int IS_PIECE_BISHOP_QUEEN[13] = {false, false, false, true, false, true, false, false, false, true, false, true, false};
const int IS_SLIDER_PIECE[13] = {false, false, false, true, true, true, false, false, false, true, true, true, false};
const int IS_NON_SLIDER_PIECE[13] = {false, false, true, false, false, false, true, false, true, false, false, false, true};

// Defines values for each piece
const int PIECE_VAL[] = {0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000};

// Defines pieces cost to play
const int PIECE_COST[] = {0, 1, 2, 3, 4, 5, 10, 1, 2, 3, 4, 5, 10};

// Defines pieces max count
const int PIECE_MAX[] = {64, 8, 2, 2, 2, 1, 1, 8, 2, 2, 2, 1, 1};

// Flags for the files
enum{FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE};
const char FILE_CHARS[] = "abcdefgh";

// Flags for the ranks
enum{RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE};
const char RANK_CHARS[] = "12345678";

// Flags for the squares of the board
enum{
  A1=21, B1, C1, D1, E1, F1, G1, H1,
  A2=31, B2, C2, D2, E2, F2, G2, H2,
  A3=41, B3, C3, D3, E3, F3, G3, H3,
  A4=51, B4, C4, D4, E4, F4, G4, H4,
  A5=61, B5, C5, D5, E5, F5, G5, H5,
  A6=71, B6, C6, D6, E6, F6, G6, H6,
  A7=81, B7, C7, D7, E7, F7, G7, H7,
  A8=91, B8, C8, D8, E8, F8, G8, H8,
  NO_SQUARE=99, OFFBOARD=100
};

const int square_from_120_board_to_64_board[BOARD_SQ_NUM] = {
  65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  65,  0,  1,  2,  3,  4,  5,  6,  7, 65,
  65,  8,  9, 10, 11, 12, 13, 14, 15, 65,
  65, 16, 17, 18, 19, 20, 21, 22, 23, 65,
  65, 24, 25, 26, 27, 28, 29, 30, 31, 65,
  65, 32, 33, 34, 35, 36, 37, 38, 39, 65,
  65, 40, 41, 42, 43, 44, 45, 46, 47, 65,
  65, 48, 49, 50, 51, 52, 53, 54, 55, 65,
  65, 56, 57, 58, 59, 60, 61, 62, 63, 65,
  65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
  65, 65, 65, 65, 65, 65, 65, 65, 65, 65
};

const int square_from_64_board_to_120_board[64] = {
  21, 22, 23, 24, 25, 26, 27, 28,
  31, 32, 33, 34, 35, 36, 37, 38,
  41, 42, 43, 44, 45, 46, 47, 48,
  51, 52, 53, 54, 55, 56, 57, 58,
  61, 62, 63, 64, 65, 66, 67, 68,
  71, 72, 73, 74, 75, 76, 77, 78,
  81, 82, 83, 84, 85, 86, 87, 88,
  91, 92, 93, 94, 95, 96, 97, 98,
};

const int SQUARE_FILE[BOARD_SQ_NUM] = {
  OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD,
  OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD,
  OFFBOARD,   FILE_A,   FILE_B,   FILE_C,   FILE_D,   FILE_E,   FILE_F,   FILE_G,   FILE_H, OFFBOARD,
  OFFBOARD,   FILE_A,   FILE_B,   FILE_C,   FILE_D,   FILE_E,   FILE_F,   FILE_G,   FILE_H, OFFBOARD,
  OFFBOARD,   FILE_A,   FILE_B,   FILE_C,   FILE_D,   FILE_E,   FILE_F,   FILE_G,   FILE_H, OFFBOARD,
  OFFBOARD,   FILE_A,   FILE_B,   FILE_C,   FILE_D,   FILE_E,   FILE_F,   FILE_G,   FILE_H, OFFBOARD,
  OFFBOARD,   FILE_A,   FILE_B,   FILE_C,   FILE_D,   FILE_E,   FILE_F,   FILE_G,   FILE_H, OFFBOARD,
  OFFBOARD,   FILE_A,   FILE_B,   FILE_C,   FILE_D,   FILE_E,   FILE_F,   FILE_G,   FILE_H, OFFBOARD,
  OFFBOARD,   FILE_A,   FILE_B,   FILE_C,   FILE_D,   FILE_E,   FILE_F,   FILE_G,   FILE_H, OFFBOARD,
  OFFBOARD,   FILE_A,   FILE_B,   FILE_C,   FILE_D,   FILE_E,   FILE_F,   FILE_G,   FILE_H, OFFBOARD,
  OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD,
  OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD
};

const int SQUARE_RANK[BOARD_SQ_NUM] = {
  OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD,
  OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD,
  OFFBOARD,   RANK_1,   RANK_1,   RANK_1,   RANK_1,   RANK_1,   RANK_1,   RANK_1,   RANK_1, OFFBOARD,
  OFFBOARD,   RANK_2,   RANK_2,   RANK_2,   RANK_2,   RANK_2,   RANK_2,   RANK_2,   RANK_2, OFFBOARD,
  OFFBOARD,   RANK_3,   RANK_3,   RANK_3,   RANK_3,   RANK_3,   RANK_3,   RANK_3,   RANK_3, OFFBOARD,
  OFFBOARD,   RANK_4,   RANK_4,   RANK_4,   RANK_4,   RANK_4,   RANK_4,   RANK_4,   RANK_4, OFFBOARD,
  OFFBOARD,   RANK_5,   RANK_5,   RANK_5,   RANK_5,   RANK_5,   RANK_5,   RANK_5,   RANK_5, OFFBOARD,
  OFFBOARD,   RANK_6,   RANK_6,   RANK_6,   RANK_6,   RANK_6,   RANK_6,   RANK_6,   RANK_6, OFFBOARD,
  OFFBOARD,   RANK_7,   RANK_7,   RANK_7,   RANK_7,   RANK_7,   RANK_7,   RANK_7,   RANK_7, OFFBOARD,
  OFFBOARD,   RANK_8,   RANK_8,   RANK_8,   RANK_8,   RANK_8,   RANK_8,   RANK_8,   RANK_8, OFFBOARD,
  OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD,
  OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD, OFFBOARD
};

#endif
