//
// Created by Stefan on 07.01.2026.
//

#include "TestGUI.h"

#include "../../Render/GUI/GuiElement.h"
#include "../../Render/GUI/CallbackFunctions.h"

void generateTestGUI(Element *guiRoot) {

    addChildElements(guiRoot,
        addChildElements(
            createElement((ElementSettings){
                .name = "Table",
                .pos = (Vec2i){100, 100},
                .padding = (Padding){10, 10, 10, 10},
                .minWidth = 100,
                .minHeight = 200,
                .color = {0.3, 0.7, 0.9},
                .onHover = defaultHoverFun,
                .draggable = true,
                .childGap = 10,
                .posMode = POS_RELATIVE,
            }),
            createElement((ElementSettings){
                .texture = "Pointer Schematic.png",
                .minWidth = 100,
                .minHeight = 20,
                .padding = (Padding){10,10,10,10},
                .text = "This is a test and yes Text Positioning must be refractored",
            }),
            TextFieldElement_new((ElementSettings){
                .padding = {10,10,10,10},
                .minHeight = 20,
                .minWidth = 100,
                .color = {0.5, 1.0, 0.3}
            }, nullptr),
            addChildElements(
                createElement((ElementSettings){
                    .minWidth = 20,
                    .minHeight = 200,
                    .color = (Vec3f){0.0f, 0.0f, 0.0f},
                    .childGap = 5,
                    .padding = (Padding){10, 10, 10, 10},
                    .maxHeight = true,
                    .onUpdate = incrementHeight,
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .minWidth = 100,
                    .minHeight = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .minWidth = 100,
                    .minHeight = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .minWidth = 100,
                    .minHeight = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .minWidth = 100,
                    .minHeight = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .minWidth = 100,
                    .minHeight = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .minWidth = 100,
                    .minHeight = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .minWidth = 100,
                    .minHeight = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .minWidth = 100,
                    .minHeight = 20,
                    .padding = (Padding){10, 10, 10, 10},
                    .text = "hoosaasas"
                })
            )
        )
    );
}
