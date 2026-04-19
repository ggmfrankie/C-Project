//
// Created by Stefan on 19.04.2026.
//

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

typedef void GameBoard;

GameBoard* game_create();
void game_destroy(GameBoard* game);

void game_input(GameBoard* game, int32_t dir);
void game_get_board(GameBoard* game, int32_t* out);

#ifdef __cplusplus
}
#endif