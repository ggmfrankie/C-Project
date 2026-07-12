//
// Created by ertls on 29.05.2026.
//

#include "Game2DGui.h"

#include "Render/GUI/GuiElement.h"

void generateGame2DGui(Element *guiRoot) {
    addChildElements(guiRoot,
        createElement((ElementSettings){
            .text = "Test",
            .color = {0.7,.0,0},
            .padding = {5,5,5,5},
            .name = "Test Panel"
        }),
        addChildElements(
            createElement((ElementSettings){
                .pos = {200, 100},
                .color = {0,1},
                .posMode = POS_RELATIVE,
                //.width = 200,
                //.height = 300,
                .maxHeight = 150,
                .padding = {5,5,5,5},
                .cornerRadius = 2,
                .childGap = 4,
                .name = "Background",
                .draggable = true
            }),
            createElement((ElementSettings){
                .text = "Heloowow",
                .minHeight = 50,
                .padding = {4,4,4,4},
                .color = {.9, .4, 0}
            }),
            createElement((ElementSettings){
                .text = "Heloowow",
                .minHeight = 50,
                .padding = {4,4,4,4},
                .color = {.9, .4, 0}
            }),
            createElement((ElementSettings){
                .text = "Heloowow",
                .minHeight = 50,
                .padding = {4,4,4,4},
                .color = {.9, .4, 0}
            })
        )
    );
}
