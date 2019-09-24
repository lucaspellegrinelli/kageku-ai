#ifndef DEFS_H
#define DEFS_H

#define ENGINE_NAME "Kai"

// Number of squares in the board
#define BOARD_SQ_NUM 120

// Int with 64 bits (8x8 board = 64 squares!)
typedef unsigned long long U64;

// Maximum number of moves a game can have
#define MAX_GAME_MOVES 2048

// Flags for each of the pieces
#define PIECE_TYPES 13
enum{EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK};
const char PIECE_CHARS[] = ".PNBRQKpnbrqk";

// Flags for the files and ranks
enum{FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE};
const char FILE_CHARS[] = "abcdefgh";
enum{RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE};
const char RANK_CHARS[] = "12345678";

// Flags for colors (BOTH - Respresents pieces of either color)
#define COLOR_COUNT 2
enum{WHITE, BLACK, BOTH};
const char COLOR_CHARS[] = "wb-";

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

#endif
