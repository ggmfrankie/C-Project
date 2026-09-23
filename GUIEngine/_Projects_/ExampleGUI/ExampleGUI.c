//
// Created by ertls on 09.09.2026.
//

#include "ExampleGUI.h"

#include "GuiColors.h"
#include "GUI/GuiElement/GuiElement.h"
#include "GUI/GuiElement/ElementTypes/ScrollArea.h"
#include "GUI/GuiElement/ElementTypes/TextField.h"
#include <stdlib.h>

#include "GUI/GuiElement/CallbackHelper.h"
#include "GUI/GuiElement/ElementTypes/Scrollbar.h"

static ElementHandle TestElement() {
    return Element_new((ElementSettings){
        .minWidth = rand()%30 + 10,
        .minHeight = rand()%50 + 10,
        .color = GUI_COLOR_DARKGRAY3,
        .text = "ja dies ist text",
        .textColor = GUI_COLOR_LIGHTGRAY3,
        .padding = {10,10,10,10},
        .cornerRadius = 5,
        .grow = rand()%2
    });
}

static ElementHandle TestElement2() {
    return Element_new((ElementSettings){
        .minWidth = 200,
        .maxWidth = 200,
        .minHeight = 10,
        .color = GUI_COLOR_DARKGRAY3,
        .text = "Dieser Text dürfte zu lang sein für das element",
        .textColor = GUI_COLOR_LIGHTGRAY3,
        .padding = {10,10,10,10},
        .cornerRadius = 5,
        .grow = true
    });
}

static void deleteElement(void*) {
    Element_delete(Element_getElement("remove"));
}

static ElementHandle TestScrollArea() {
    return Element_new((ElementSettings){
                .color = GUI_COLOR_DARKGRAY2,
                .padding = {10,10,10, 10},
                .cornerRadius = 10,
                .flexGrow = 1.0
            },
                ScrollArea_new((ScrollAreaSettings){
                    .pos = {},
                    .height = rand()%300 + 100,
                    .backgroundColor = GUI_COLOR_DARKGRAY2,
                    .sliderColor = GUI_COLOR_BLACK,
                    .railColor = GUI_COLOR_DARKGRAY1,
                    .childGap = 5,
                    .padding = {5},
                    .flexGrow = 1,
                    .name = "remove",
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
                    }, TextField_runTask),
                    Element_new((ElementSettings){
                        .color = GUI_COLOR_AZURE,
                        .text = "Press Me!",
                        .onClick = runTaskFun,
                        .task = {deleteElement}
                    })
                )
            );
}

static ElementHandle teste() {
    return Element_new((ElementSettings){
        .minWidth = 100,
        .minHeight = 100,
        .color = GUI_COLOR_DARKGRAY2,
        .cornerRadius = 10,
        .flexGrow = 1.0,
        .layoutDirection = LAYOUT_RIGHT
    },
        Scrollbar_new((ScrollbarSettings){
            .pos = {},
            .railWidth = 10,
            .sliderHeight = 20,
            .sliderColor = GUI_COLOR_LIGHTGRAY2,
            .railColor = GUI_COLOR_DARKGRAY3
        })
    );
}

void ExampleGui_generate_1(Element* root) {
    addChildElements(root,
        Element_new((ElementSettings){
            .color = GUI_COLOR_DARKGRAY1,
            .grow = true,
            .flexGrow = 1.0,
            .minWidth = 100,
            .minHeight = 100,
            .padding = {10,10,10,10},
            .childGap = 10
        },
            TestScrollArea(),
            TestElement(),
            TestElement(),
            TestElement(),
            TestElement(),
            teste(),
            TestElement(),
            TestElement(),
            TestElement(),
            TestElement(),
            TestElement(),
            TestElement(),
            TestElement2()
        )
    );
}

static ElementHandle createColoredSquares(const int row, const int col, ElementSettings es) {
    es.color = (row+col) % 2 ? GUI_COLOR_DARKGREEN : GUI_COLOR_EMERALD;


    const ElementHandle square = createElement(es);

    return square;
}

void ExampleGui_generate_2(Element* root) {
    addChildElements(root,
        Element_new((ElementSettings){
            .color = GUI_COLOR_DARKGRAY1,
            .grow = true,
            .flexGrow = 1.0,
            .minWidth = 100,
            .minHeight = 100,
            .padding = {10,10,10,10},
            .childGap = 10
        },
            addChildrenAsGridWithGenerator(
            (ElementSettings){
                .color = GUI_COLOR_ROSE,
                .minWidth = 1000,
                .minHeight = 1000,
                .childGap = 10,
                .padding = {10,10,10,10}
            },
                (ElementSettings){
                    .color = GUI_COLOR_ROSE
                },
            8, 8, createColoredSquares
            )
        )
    );
}
