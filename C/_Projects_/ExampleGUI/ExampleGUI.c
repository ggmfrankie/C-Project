//
// Created by ertls on 09.09.2026.
//

#include "ExampleGUI.h"

#include "GuiColors.h"
#include "GUI/GuiElement/GuiElement.h"
#include "GUI/GuiElement/ElementTypes/ScrollArea.h"
#include "GUI/GuiElement/ElementTypes/TextField.h"

static ElementHandle TestElement() {
    return Element_new((ElementSettings){
        .minWidth = 50,
        .minHeight = 10,
        .color = GUI_COLOR_DARKGRAY3,
        .text = "ja dies ist text",
        .textColor = GUI_COLOR_LIGHTGRAY3,
        .padding = {10,10,10,10},
        .cornerRadius = 5,
    });
}

static ElementHandle TestScrollArea() {
    return Element_new((ElementSettings){
                .color = GUI_COLOR_DARKGRAY2,
                .draggable = true,
                .layoutDirection = LAYOUT_RIGHT,
                .padding = {10,10,10, 10},
                .cornerRadius = 10
            },
                ScrollArea_new((ScrollAreaSettings){
                    .pos = {},
                    .height = 180,
                    .backgroundColor = GUI_COLOR_DARKGRAY2,
                    .sliderColor = GUI_COLOR_BLACK,
                    .railColor = GUI_COLOR_DARKGRAY1,
                    .childGap = 5,
                    .padding = {5}
                },
                    TestElement(),
                    TestElement(),
                    TestElement(),
                    TestElement(),
                    TestElement(),
                    TestElement(),
                    TestElement(),
                    TestElement(),
                    TestElement(),
                    TestElement(),
                    TestElement(),
                    TestElement(),
                    TextField_new((ElementSettings){
                        .padding = {10,10,10,10},
                        .minHeight = 20,
                        .minWidth = 100,
                        .color = {0.88f, 0.88f, 0.91f},
                        .cornerRadius = 10
                    }, TextField_runTask)
                )
            );
}

void ExampleGui_generate(Element* root) {
    addChildElements(root,
        Element_new((ElementSettings){
            .color = GUI_COLOR_DARKGRAY1,
            .grow = true,
            .flexGrow = 1.0,
            .minWidth = 100,
            .minHeight = 100,
        },
            TestScrollArea(),
            TestScrollArea(),
            TestScrollArea()
        )
    );
}
