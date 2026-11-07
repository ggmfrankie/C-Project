//
// Created by Stefan on 07.11.2026.
//

#include "GameGui.h"

#include "GuiColors.h"
#include "glad/gl.h"

#include "GUI/GuiElement/GuiElement.h"
#include "GUI/GuiElement/ElementTypes/ScrollArea.h"

static bool ggmTanks_onClickToggleDebugView(Element*) {
    static bool debug = false;
    if ((debug = !debug)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    return true;
}

static ElementHandle ggmTanks_simpleButton(const char* text, const char* name, bool (*onClick)(Element* element)) {
    return Element_new((ElementSettings){
        .color = GUI_COLOR_DARKGRAY(0.090),
        .padding = {5, 5, 5, 5},
        .childGap = 5,
        .cornerRadius = 7
    },
        Element_new((ElementSettings){
            .color = GUI_COLOR_DARKGRAY(0.130),
            .cornerRadius = 5,
            .padding = {5,5,5,7},
            .text = text,
            .textColor = GUI_COLOR_LIGHTGRAY1,
            .canBeHovered = true,
            .onClick = onClick,
            .name = name,
        })
    );
}

void ggmTanks_createDebugMenu(Element* root) {
    Element_addChildren(root,
        Element_new((ElementSettings){
            .cornerRadius = 10,
            .color = GUI_COLOR_DARKGRAY(0.030),
            .childGap = 10,
            .padding = {10, 10, 10, 10},
            .draggable = true,
            .posMode = POS_RELATIVE
        },
            Element_new((ElementSettings){
                .transparency = 1.0,
                .text = "Debug Menu",
                .textColor = GUI_COLOR_LIGHTGRAY1,
                .notSelectable = true
            }),

            ggmTanks_simpleButton("Toggle debug view", nullptr, ggmTanks_onClickToggleDebugView)
        )
    );
}
