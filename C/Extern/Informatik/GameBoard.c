//
// Created by Stefan on 04.11.2025.
//

#include "GameBoard.h"

#include <bemapiset.h>
#include <stdio.h>
#include "../../Utils/CString.h"
#include "../../Utils/DataStructures.h"
#pragma GCC diagnostic ignored "-Wsign-compare"

bool moveBishopTo(GameBoard *board, int x, int y);

#ifdef HEAP_BOARD
int** mallocBoard(const size_t sizeX, const size_t sizeY) {
    // ReSharper disable once CppDFAMemoryLeak
    int** content = malloc(sizeof (int*)* sizeX);

    for (size_t i = 0; i < sizeX; i++) {
        content[i] = malloc(sizeof(int)* sizeY);
    }
    return content;
}

//redundant
void deleteBoard(GameBoard *board) {
    for (int i = 0; i < board->sizeX; i++) {
        free(board->board[i]);
    }
    free(board->board);
}
#endif

void clearBoard(GameBoard *board) {
    for (int i = 0; i < board->sizeX; i++) {
        for (int j = 0; j < board->sizeY; j++) {
            board->board[i][j] = 0;
        }
    }
}

void initBoard(GameBoard *board) {
    clearBoard(board);
    moveBishopTo(board, (int)board->sizeX/2, (int)board->sizeY/2);
}

GameBoard createBoard() {
#ifdef HEAP_BOARD
    return (GameBoard) {
        .sizeX = WIDTH,
        .sizeY = HEIGHT,
        .board = mallocBoard(WIDTH, HEIGHT)
    };
#else

    return (GameBoard) {
        .sizeX = WIDTH,
        .sizeY = HEIGHT
    };
#endif

}

int Board_get(const GameBoard *board, const int x, const int y) {
    if (x < 0 || y < 0) return -1;
    if (x >= board->sizeX || y >= board->sizeY) return -1;
    return board->board[x][y];
}

void Board_set(GameBoard *board, const int x, const int y, const int value) {
    if (x < 0 || y < 0) return;
    if (x >= board->sizeX || y >= board->sizeY) return;
    board->board[x][y] = value;
}

bool moveBishopTo(GameBoard *board, const int x, const int y) {
    if (x < 0 || y < 0) return false;
    if (x >= board->sizeX || y >= board->sizeY) return false;

    board->pos.x = x;
    board->pos.y = y;

    board->board[x][y]++;
    return true;
}

bool moveBishop(GameBoard *board, const Direction d) {
    const Vec2i pos = board->pos;
    bool success = false;
    switch (d) {
        case left_up: {
            success = moveBishopTo(board, max(pos.x-1, 0), max(pos.y-1, 0));
            break;
        }
        case right_up: {
            success = moveBishopTo(board, min(pos.x+1, board->sizeX-1), max(pos.y-1, 0));
            break;
        }
        case left_down: {
            success = moveBishopTo(board, max(pos.x-1, 0), min(pos.y+1, board->sizeY-1));
            break;
        }
        case right_down: {
            success = moveBishopTo(board, min(pos.x+1, board->sizeX-1), min(pos.y+1, board->sizeY-1));
            break;
        }
        default: return false;
    }

    return success;
}

void moveAndPrintBoard(GameBoard *board, const Direction d) {
    if (moveBishop(board, d)) {
        printBoard(board);
    }
}

char getBoard(const GameBoard *board, const size_t x, const size_t y) {
    static char mapping[MAPLEN] = {' ','.','o','+','=','*','B','0','X','@','%','&','#','/','^'};
    return mapping[board->board[x][y] % MAPLEN];
}

void printBoard(const GameBoard *board) {
    const size_t size = board->sizeX + 1;
    char wall[size];
    char_Array horizontalWall = char_newArray(wall, size);
    char_Array_setAll(&horizontalWall, '-');
    char_Array_set(&horizontalWall, board->sizeX, '\0');

    printf("+%s+\n", horizontalWall.content);

    for (size_t i = 0; i < board->sizeY; i++) {
        printf("|");
        for (size_t j = 0; j < board->sizeX; j++) {
            printf("%c",getBoard(board, j, i));
        }
        printf("|\n");
    }

    printf("+%s+\n", horizontalWall.content);
}