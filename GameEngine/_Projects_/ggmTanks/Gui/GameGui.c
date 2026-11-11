//
// Created by Stefan on 07.11.2026.
//

#include "GameGui.h"

#include "GuiColors.h"
#include "GLFW/glfw3.h"

#include "GUI/GuiElement/GuiElement.h"
#include "GuiCallbacks.hpp"

static ElementHandle ggmTanks_simpleButton(const char* text, const char* name, bool (*onClick)(Element* element), const char* icon) {
    return Element_new((ElementSettings){
        .color = GUI_COLOR_GRAYSCALE(0.190),
        .padding = {5, 5, 5, 5},
        .childGap = 5,
        .cornerRadius = 7,
        .layoutDirection = LAYOUT_RIGHT,
        .canBeHovered = true,
        .onClick = onClick,
        .name = name,
        .transparency = .4
    },
        Element_new((ElementSettings){
            .texture = icon,
            .minWidth = 25,
            .minHeight = 30,
            .color = GUI_COLOR_WHITE,
            .grow = true,
            .notSelectable = true
        }),
        Element_new((ElementSettings){
            .color = GUI_COLOR_GRAYSCALE(0.230),
            .cornerRadius = 5,
            .padding = {5,5,5,7},
            .text = text,
            .textColor = GUI_COLOR_LIGHTGRAY1,
            .transparency = 1,
            .notSelectable = true
        })
    );
}

void ggmTanks_createDebugMenu(Element* root) {
    gui_addTextures(
        "Icons/WhiteX.png",
        "Pointer Schematic.png",
        "Icons/RefreshIcon.png",
        "Icons/DebugIcon.png",
        "Icons/DebugBug.png"
    );

    Element_addChildren(root,
        Element_new((ElementSettings){
            .cornerRadius = 10,
            .color = GUI_COLOR_GRAYSCALE(0.130),
            .childGap = 10,
            .padding = {10, 10, 10, 10},
            .draggable = true,
            .posMode = POS_RELATIVE,
            .transparency = 0.10
        },
            Element_new((ElementSettings){
                .transparency = 1.0,
                .text = "Debug Menu",
                .textColor = GUI_COLOR_LIGHTGRAY1,
                .notSelectable = true
            }),

            ggmTanks_simpleButton("Toggle debug view", nullptr, ggmTanks_onClick_toggleDebugView, "Icons/DebugIcon.png"),
            ggmTanks_simpleButton("Reload Terrain", "terrain_reload", nullptr, "Icons/RefreshIcon.png"),
            ggmTanks_simpleButton("Close Window", nullptr, ggmTanks_onClick_closeWindow, "Icons/WhiteX.png")
        )
    );
}
