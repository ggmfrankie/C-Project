//
// Created by Stefan on 07.01.2026.
//

#include "ChessGame.h"

#include "../../Render/Engine.h"
#include "../../Render/GUI/CallbackFunctions.h"
#define COOL_COLOR (Vec3f){.2, .3, .3}
#define COLOR_WHITE (Vec3f){1, 1, 1}
#define COLOR_BLACK (Vec3f){0, 0, 0}

void createStartScreen(Element* root);
void createChessBoard(Element* root);

void createChessGUI(Element* root) {
    createStartScreen(root);
    createChessBoard(root);
}

void startChessGameTask(void* nix) {
    Element* mainMenu = *Hashmap_Element_get(&g_Hashmap, "start screen");
    Element* chessBoard = *Hashmap_Element_get(&g_Hashmap, "game board");
    setVisible(mainMenu, false);
    setVisible(chessBoard, true);
}

void closeProgram(void* nix) {
    exit(69);
}

Element* createChessSquares(ElementSettings es) {
    static int row = 0;
    static int column = 0;
    es.color = (((row+column) % 2) ? COLOR_WHITE : COLOR_BLACK);

    if (row == 7) {
        column++;
        row = 0;
    } else {
        row++;
    }
    return createElement(es);
}

void createChessBoard(Element* root) {

    addChildElements(root,
        addChildrenAsGridWithGenerator(
            (ElementSettings){
                .color = COOL_COLOR,
                .posMode = POS_ABSOLUTE,
                .height = 400,
                .width = 400,
                .childGap = 0,
                .padding = (Padding){10,10,10,10},
                .pos = (Vec2i){800, 200},
                .name = "game board",
                .onHover = hoverAndDragFunction
            },
            (ElementSettings){
                .color = COLOR_WHITE,
            }, 8, 8,
            createChessSquares
            )
    );
    Element* chessBoard = *Hashmap_Element_get(&g_Hashmap, "game board");
    chessBoard->isActive = false;
}

void createStartScreen(Element* root) {
    const Task startChessGame = (Task){startChessGameTask, NULL};
    addChildElements(root,
        addChildElements(
            createElement(
                (ElementSettings){
                    .color = COOL_COLOR,
                    .posMode = POS_ABSOLUTE,
                    .childGap = 10,
                    .padding = (Padding){10,10,10,10},
                    .pos = (Vec2i){200, 200},
                    .name = "start screen"
                }
            ),
            createElement(
                (ElementSettings){
                    .color = (Vec3f){.2, .3, .3},
                    .text = "Welcome to my Chess Game"
                }
            ),
            createElement(
                (ElementSettings){
                    .color = (Vec3f){.3, .3, .3},
                    .padding = (Padding){10,10,10,10},
                    .text = "Start",
                    .onHover = defaultHoverFunction,
                    .onClick = runTaskFunction,
                    .task = startChessGame
                }
            ),
            createElement(
                (ElementSettings){
                    .color = (Vec3f){.8, .0, .0},
                    .padding = (Padding){10,10,10,10},
                    .text = "End it all",
                    .onHover = defaultHoverFunction,
                    .onClick = runTaskFunction,
                    .task = closeProgram
                }
            )
        )
    );
}