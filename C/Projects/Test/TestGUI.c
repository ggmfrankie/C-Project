//
// Created by Stefan on 07.01.2026.
//

#include "TestGUI.h"

#include "../../Render/GUI/GuiElement.h"
#include "../../Extern/Informatik/Namensliste.h"
#include "../../Render/GUI/CallbackFunctions.h"

void generateTestGUI(Element *guiRoot) {

    addChildElements(guiRoot,
        addChildElements(
            createElement((ElementSettings){
                .name = "Table",
                .pos = (Vec2i){100, 100},
                .padding = (Padding){10, 10, 10, 10},
                .width = 100,
                .height = 200,
                .color = {0.3, 0.7, 0.9},
                .onHover = defaultHoverFun,
                .draggable = true,
                .childGap = 10,
                .posMode = POS_ABSOLUTE,
            }),
            createElement((ElementSettings){
                .texture = "Pointer Schematic.png",
                .width = 100,
                .height = 20,
                .padding = (Padding){10,10,10,10},
                .text = "This is a test and yes Text Positioning must be refractored",
            }),
            createTextFieldElement((ElementSettings){
                .padding = {10,10,10,10},
                .height = 20,
                .width = 100,
                .color = {0.5, 1.0, 0.3}
            }),
            addChildElements(
                createElement((ElementSettings){
                    .width = 20,
                    .height = 200,
                    .color = (Vec3f){0.0f, 0.0f, 0.0f},
                    .childGap = 5,
                    .padding = (Padding){10, 10, 10, 10},
                    .fixedHeight = true,
                    .onUpdate = incrementHeight,
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = defaultHoverFun,
                    .onClick = runTaskFun,
                    .draggable = true,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                })
            )
        )
    );
}
