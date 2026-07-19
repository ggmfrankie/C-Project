//
// Created by ertls on 29.05.2026.
//
#include "Render/GUI/GuiElement.h"
#include "Game2DGui.h"

void generateGame2DGui(Element *guiRoot) {
    addChildElements(guiRoot,
            Element_new((ElementSettings){
                .text = "Heloowow",
                .padding = {4,4,4,4},
                .color = {.9, .4, 0}
            },
                Element_new((ElementSettings){
                    .text = "Heloowow",
                    .padding = {4,4,4,4},
                    .color = {.9, .4, 0}
                }),
                Element_new((ElementSettings){
                    .text = "Heloowow",
                    .padding = {4,4,4,4},
                    .color = {.9, .4, 0}
                })
            )
    );
}
