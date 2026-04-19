//
// Created by Stefan on 19.04.2026.
//

#include "TFE.h"
#include "../Dependencies/RustGame/TFE.h"

#include <stdio.h>

#include "Render/GUI/CallbackFunctions.h"
#include "Render/GUI/GuiElement.h"

void* game = nullptr;

static void TFE_onKeyPress(int key, int scancode, int action, int mods) {

}

void TFE_createGame() {
    game = game_create();

}

Element* TFE_createSquare(int, int, ElementSettings es) {
    return addChildElements(
        createElement(es),
        createElement((ElementSettings){
            .text = "110",
            .pos = {20, 30},
            .posMode = POS_ABSOLUTE,
            .transparency = 1.0f
        })
    );
}

void TFE_createGUI(Element* root) {
    addChildElements(root,
        addChildElements(
            createElement((ElementSettings){
                .pos = {400, 400},
                .cornerRadius = 10,
                .draggable = true,
                .color = {0.4, 0.6, 0.7},
                .name = "TFE_board"
            }),
            createElement((ElementSettings){
                .text = "Hello to 2048",
                .transparency = 1.0f,
                .notSelectable = true
            }),
            addChildrenAsGridWithGenerator(
                (ElementSettings){
                    .color = {0.4, 0.6, 0.7},
                    .width = 400,
                    .height = 400,
                    .fixedWidth = true,
                    .fixedHeight = true,
                    .padding = {10,10,10,10},
                    .childGap = 10
                },

                (ElementSettings){
                    .color = {0.3, 0.3, 0.3},
                    .cornerRadius = 20
                }, 4, 4, TFE_createSquare
            )
        )
    );
    setActive(getElement("TFE_board"), false);
}

void TFE_run() {

    game_input(game, 0);
    int board[16];
    game_get_board(game, board);
    game_destroy(game);
    game = nullptr;

    for (int i = 0; i < 16; i++) {
        printf("%i ", board[i]);
    }
}


