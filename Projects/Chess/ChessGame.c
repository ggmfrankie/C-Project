//
// Created by Stefan on 07.01.2026.
//

#include "ChessGame.h"
#include "../../Render/GUI/CallbackFunctions.h"
#define COOL_COLOR (Vec3f){.2, .3, .3}
void createStartScreen(Element* root);

void createChessGUI(Element* root) {
    createStartScreen(root);

}

void startChessGameTask(void* nix) {

}

void closeProgram(void* nix) {
    exit(69);
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