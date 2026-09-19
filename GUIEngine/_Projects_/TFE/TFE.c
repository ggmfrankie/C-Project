//
// Created by Stefan on 19.04.2026.
//

#include "TFE.h"
#include "../Dependencies/RustGame/TFE.h"

#include <stdio.h>

#include "GuiInterface.h"
#include "GUI/GuiElement/GuiElement.h"

void* game = nullptr;

static void TFE_onKeyPress(int key, int scancode, int action, int mods) {
    puts("some key pressed");
}

void TFE_newGame() {
#if 0

    if (game) game_destroy(game);
    game = game_create();
    gui_onKeyPressCallback(TFE_onKeyPress);
#endif
}

Element* TFE_createSquare(int, int, ElementSettings es) {
    return nullptr;
}

void TFE_createGUI(Element* root) {

}

void TFE_run() {
#if 0
    game_input(game, 0);
    int board[16];
    game_get_board(game, board);
    game_destroy(game);
    game = nullptr;

    for (int i = 0; i < 16; i++) {
        printf("%i ", board[i]);
    }
#endif

}


