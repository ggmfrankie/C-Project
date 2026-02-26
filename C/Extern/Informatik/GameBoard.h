//
// Created by Stefan on 04.11.2025.
//

#ifndef C_GAMEBOARD_H
#define C_GAMEBOARD_H
#include <stdbool.h>
#include <stdlib.h>
#include "../../Utils/Vector.h"

#define HEIGHT 7
#define WIDTH 15

#define MAPLEN 16
//#define HEAP_BOARD

#ifdef HEAP_BOARD
typedef struct gameBoard {
    size_t sizeX;
    size_t sizeY;
    Vec2i pos;
    int **board;
} GameBoard;
#else

typedef struct gameBoard {
    size_t sizeX;
    size_t sizeY;
    Vec2i pos;
    int board[WIDTH][HEIGHT];
} GameBoard;
#endif


typedef enum Direction {
    left_up = 0,
    right_up = 1,
    left_down = 2,
    right_down = 3
} Direction;

GameBoard createBoard();
bool moveBishop(GameBoard *board, Direction d);
void moveAndPrintBoard(GameBoard *board, Direction d);
void printBoard(const GameBoard *board);
void initBoard(GameBoard *board);
void deleteBoard(GameBoard *board);
#endif //C_GAMEBOARD_H