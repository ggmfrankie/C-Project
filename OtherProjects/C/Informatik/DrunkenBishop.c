//
// Created by Stefan on 04.11.2025.
//

#include "DrunkenBishop.h"

#include <stdio.h>

#include "GameBoard.h"
#include "../../../C/Utils/Makros/Makros.h"

#include "Utils/Os/Time.h"
#include "../../../C/Utils/Makros/Makros.h"
#include "Utils/Os/Time.h"

void runTest() {
    GameBoard board = createBoard();
    initBoard(&board);

    printBoard(&board);

    moveAndPrintBoard(&board, left_up);
    moveAndPrintBoard(&board, left_up);
    moveAndPrintBoard(&board, left_down);
    moveAndPrintBoard(&board, left_down);
    moveAndPrintBoard(&board, right_down);
    moveAndPrintBoard(&board, right_down);
    moveAndPrintBoard(&board, right_up);
    moveAndPrintBoard(&board, right_up);
    for(int __i = 0; __i < (10); __i++){ moveAndPrintBoard(&board, right_down); };
}

void drawWithBishop(unsigned long long input) {
    const time_t startingTime = now_ns();
    constexpr int steps = sizeof input * 4;
    GameBoard board = createBoard();
    initBoard(&board);

    for (int i = 0; i < steps; i++) {
        const Direction dir = (Direction)(input % 4);
        input >>= 2;
        moveBishop(&board, dir);
    }

    const time_t elapsedTime = now_ns()-startingTime;
    printf("Time to calculate was: %lli\n", elapsedTime);
    printBoard(&board);
#ifdef HEAP_BOARD
    deleteBoard(&board);
#endif

}
