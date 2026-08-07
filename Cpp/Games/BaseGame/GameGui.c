//
// Created by Stefan on 27.02.2026.
//

#include "GameGui.h"

#include <stdio.h>
#include <stdlib.h>

#include "Render/GUI/CallbackFunctions.h"
#include "GuiTasks.hpp"
#include "Projects/Chess/ChessGame.h"
#include "Projects/TFE/TFE.h"
#include "Render/GUI/GuiElement.h"
#include "Render/GUI/ElementTypes/TextField.h"


void quitTask(void*) {
    puts("Bye World");
    exit(0);
}

void generateDebugGui(Element* guiRoot) {
    addChildElements(guiRoot,
        addChildElements(
            createElement((ElementSettings){
                .minHeight = 20,
                .maxHeight = true,
                .cantBeSelected = true,
                .invisible = true,
                .color = {},
                .posMode = POS_RELATIVE
            }),
            createElement((ElementSettings){
                .minWidth = 20,
                .text = "fps",
                .cantBeSelected = true,
                .invisible = true,
                .name = "fps display"
            })
        )
    );
}

void generateMainMenuGui(Element* guiRoot) {
    static float fov = 70.0f;
    static float fov2 = 90.0f;
    const Task changeFov70 = {Engine_changeFOV, &fov};
    const Task changeFov90 = {Engine_changeFOV, &fov2};

    const Task runCommand = {Engine_runCommand};

    addChildElements(guiRoot,
        Element_new((ElementSettings){
            .name = "Home Screen",
            .pos = (Vec2i){0, 0},
            .padding = (Padding){10, 10, 10, 10},
            .color = {0.78f, 0.78f, 0.78f},
            .draggable = true,
            .childGap = 10,
            .posMode = POS_RELATIVE,
            .transparency = 0.5,
            .cornerRadius = 20.f,
            .minWidth = 0,
            .minHeight = 0
        },
            Element_new((ElementSettings){
                .invisible = true,
                .cantBeSelected = true,
                .text = "Home Screen",
            }),
            TextField_new((ElementSettings){
                .padding = {10,10,10,10},
                .minHeight = 20,
                .minWidth = 100,
                .color = {0.88f, 0.88f, 0.91f},
                .cornerRadius = 10,
                .task = runCommand

            }, TextField_runTask),
            Element_new((ElementSettings){
                .padding = {10,10,10,10},
                .minHeight = 20,
                .minWidth = 100,
                .color = {0.88f, 0.88f, 0.91f},
                .onClick = runTaskFun,
                .canBeHovered = true,
                .task = changeFov70,
                .text = "change fov to 70",
                .name = "btn_fov_70",
                .cornerRadius = 10
            }),
            Element_new((ElementSettings){
                .padding = {10,10,10,10},
                .minHeight = 20,
                .minWidth = 100,
                .color = {0.88f, 0.88f, 0.91f},
                .onClick = runTaskFun,
                .canBeHovered = true,
                .task = changeFov90,
                .text = "change fov to 90",
                .name = "btn_fov_90",
                .cornerRadius = 10
            }),
            Element_new((ElementSettings){
                .text = "Start Chess",
                .padding = {10,10,10,10},
                .color = {0.88f, 0.88f, 0.91f},
                .canBeHovered = true,
                .onClick = runTaskFun,
                .task = {update_chessGame},
                .cornerRadius = 10
            }),
            Element_new((ElementSettings){
                .text = "Start 2048",
                .padding = {10,10,10,10},
                .color = {0.88f, 0.88f, 0.91f},
                .canBeHovered = true,
                .onClick = runTaskFun,
                .task = {TFE_gameCreate},
                .cornerRadius = 10
            }),
            Element_new((ElementSettings){
                .text = "Quit",
                .padding = {10,10,10,10},
                .color = {0.88f, 0.88f, 0.91f},
                .canBeHovered = true,
                .onClick = runTaskFun,
                .task = quitTask,
                .cornerRadius = 10
            })
        )
    );

    Element_setActive(Element_getElement("Home Screen"), false);
}

void generateGUI(Element* guiRoot) {
    generateDebugGui(guiRoot);
    generateMainMenuGui(guiRoot);
    createChessGUI(guiRoot);
    TFE_createGUI(guiRoot);
}
