//
// Created by ertls on 29.05.2026.
//

#include "Game2DGui.h"

#include "Render/GUI/GuiElement.h"

void generateGame2DGui(Element *guiRoot) {
    addChildElements(guiRoot,
            createElement((ElementSettings){
                .text = "Heloowow",
                .padding = {4,4,4,4},
                .color = {.9, .4, 0}
            }),
            createElement((ElementSettings){
                .text = "Heloowow",
                .padding = {4,4,4,4},
                .color = {.9, .4, 0}
            }),
            createElement((ElementSettings){
                .text = "Heloowow",
                .padding = {4,4,4,4},
                .color = {.9, .4, 0}
            }),
            createElement((ElementSettings){
                .text = "Heloowow",
                .padding = {4,4,4,4},
                .color = {.9, .4, 0}
            }),
            createElement((ElementSettings){
                .text = "Heloowow",
                .padding = {4,4,4,4},
                .color = {.9, .4, 0}
            })
    );
}
