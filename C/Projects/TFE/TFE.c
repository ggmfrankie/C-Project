//
// Created by Stefan on 19.04.2026.
//

#include "TFE.h"
#include "../Dependencies/RustGame/TFE.h"

#include <stdio.h>

void TFE_run() {
    void* game = game_create();
    game_input(game, 0);
    int board[16];
    game_get_board(game, board);
    game_destroy(game);
    game = nullptr;

    for (int i = 0; i < 16; i++) {
        printf("%i ", board[i]);
    }
}
