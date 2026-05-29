//
// Created by ertls on 29.05.2026.
//

#include "Game2DGui.h"

#include "Render/GUI/GuiElement.h"

void generateGame2DGui(Element *guiRoot) {
    addChildElements(guiRoot,
        createElement((ElementSettings){
            .text = "Test",
            .color = {0.7,.0,0}
        }),
        addChildElements(
            createElement((ElementSettings){
                .pos = {200, 100},
                .color = {0,1},
                .posMode = POS_RELATIVE,
                .width = 200,
                .height = 200
            }),
            createElement((ElementSettings){
                .text = "Heloowow",
                .color = {.9, .4, 0}
            }),
            createElement((ElementSettings){
                .text = "Heloowow",
                .color = {.9, .4, 0}
            }),
            createElement((ElementSettings){
                .text = "Heloowow",
                .color = {.9, .4, 0}
            })
        )
    );
}
