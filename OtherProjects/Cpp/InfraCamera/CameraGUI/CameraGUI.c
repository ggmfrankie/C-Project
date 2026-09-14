//
// Created by ertls on 14.09.2026.
//

#include "CameraGUI.h"

#include "GuiColors.h"
#include "GuiInterface.h"
#include "GUI/GuiElement/CallbackHelper.h"
#include "Gui/GuiElement/GuiElement.h"

void pollData(void*);

static ElementHandle createDataDisplay(char* name) {
    return Element_new((ElementSettings){
        .color = GUI_COLOR_LIGHTGRAY3,
        .name = name,
        .text = name,
        .padding = {10,10,10,10},
        .minWidth = 150,
        .cornerRadius = 10
    });
}

static ElementHandle createSingleDisplay(char* name) {
    return Element_new((ElementSettings){
        .color = GUI_COLOR_LIGHTGRAY2,
        .padding = {10,10,10,10},
        .childGap = 5,
        .flexGrow = .5
    },
        createDataDisplay(name)
    );
}

static ElementHandle createBoxDisplay() {
    return Element_new((ElementSettings){
        .color = GUI_COLOR_LIGHTGRAY2,
        .padding = {10,10,10,10},
        .childGap = 5
    },
        createDataDisplay("Box1 Min"),
        createDataDisplay("Box1 Max"),
        createDataDisplay("Box1 Avg")
    );
}

static ElementHandle content() {
    return Element_new((ElementSettings){
        .invisible = true,
        .notSelectable = true,
        .layoutDirection = LAYOUT_RIGHT,
        .childGap = 10,
        .grow = true,
        .flexGrow = 0.5
        },
            Element_new((ElementSettings){
                .invisible = true,
                .notSelectable = true,
                .grow = true,
                .childGap = 5
            },
                createSingleDisplay("Spot1"),
                createSingleDisplay("Spot2"),
                createSingleDisplay("Spot3"),

                createBoxDisplay()
            ),
            Element_new((ElementSettings){
                .color = GUI_COLOR_LIGHTGRAY2,
                .notSelectable = true,
                .grow = true,
                .flexGrow = 0.5f
            },
                Element_new((ElementSettings){
                    .texture = "Camera/AX8.png",
                    .color = GUI_COLOR_WHITE,
                    .minWidth = 100,
                    .minHeight = 100,
                    .grow = true,
                    .flexGrow = 0.5f
                })
            )
    );
}

static ElementHandle createMeasurementBox() {
    return Element_new((ElementSettings){
                .color = GUI_COLOR_LIGHTGRAY1,
                .cornerRadius = 10,
                .padding = {10,10,10,10},
                .childGap = 5,
                .grow = true,
                .flexGrow = 0.5
            },
                Element_new((ElementSettings){
                    .transparency = 1.0,
                    .text = "Measurements",
                    .textScale = 1.1,
                    .notSelectable = true,
                    .padding = {0,0,0,10}
                }),
                content()
            );
}

static ElementHandle createButton(char* text, Vec3f color) {
    return Element_new((ElementSettings){
        .text = text,
        .color = color,
        .padding = {10,10,10,10},
        .cornerRadius = 10,
        .canBeHovered = true,
        .onClick = runTaskFun,
        .task = {pollData}
    });
}

static ElementHandle createControlBox() {
    return Element_new((ElementSettings){
                .color = GUI_COLOR_LIGHTGRAY1,
                .cornerRadius = 10,
                .padding = {10,10,10,10},
                .childGap = 5,
                .flexGrow = 0.1,
                .grow = true
            },
                Element_new((ElementSettings){
                    .transparency = 1.0,
                    .text = "Controls",
                    .textScale = 1.1,
                    .notSelectable = true,
                    .padding = {0,0,0,10}
                }),
                    Element_new((ElementSettings){
                        .color = GUI_COLOR_LIGHTGRAY2,
                        .padding = {10,10,10,10},
                        .childGap = 5,
                        .layoutDirection = LAYOUT_RIGHT
                    },
                        createButton("Poll data", GUI_COLOR_DARKGREEN),
                        createButton("Quit", GUI_COLOR_DARKGREEN)
                    )
            );
}

static void pollRepeat(Element*) {
    pollData(nullptr);
}

void Camera_generateGUI(Element* root) {
    gui_addTextures("Camera/AX8.png");
    addChildElements(root,
        Element_new((ElementSettings){
            .color = GUI_COLOR_LIGHTGRAY3,
            .padding = {5,5,5,5},

            .grow = true,
            .flexGrow = 1.0,
            .childGap = 5,
        },
            createMeasurementBox(),
            createControlBox()
        )
    );
}
