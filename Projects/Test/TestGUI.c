//
// Created by Stefan on 07.01.2026.
//

#include "TestGUI.h"

#include "../../Render/GUI/GuiElement.h"
#include "../../Extern/Informatik/Namensliste.h"
#include "../../Render/GUI/CallbackFunctions.h"

void generateTestGUI(Element *guiRoot) {

    Texture* blackButton = loadTextureFromPng("GrayBox.png");
    Texture* pointerSchematic = loadTextureFromPng("Pointer Schematic.png");

    const Task changeButtonTextTask = {.func = changeElementText, .userdata = THIS_ELEMENT};
    const Task nameShenanigans = {.func = namensliste_Aufgabe, .userdata = NULL};

    Font font = loadFontAtlas("ARIAL.TTF");
    Texture tex = font.fontAtlas;

    addChildElements(guiRoot,
        addChildElements(guiAddSimpleRectangle_Color((Vec2i){300, 100}, 230, pointerSchematic->height, (Vec3f){0.0f, 0.0f, 0.0f}),
            guiAddSimpleRectangle_Texture(fitMode, 230, pointerSchematic->height, blackButton),
            guiAddSimpleButton_Color(fitMode, 100, 100, (Vec3f){0.2f, 0.2f, 0.3f}, changeButtonTextTask,  "Hello World and all others too")
            ,guiAddSimpleButton_Texture(fitMode, 100, 100, pointerSchematic, nameShenanigans,  "Run the namensliste Test")
            ,guiAddSimpleButton_Color(fitMode, 100, 100, (Vec3f){0.9f, 0.2f, 0.3f}, changeButtonTextTask,  "Hello World and all others too")
            ,guiAddSimpleButton_Color(fitMode, 100, 100, (Vec3f){0.9f, 0.2f, 0.3f}, changeButtonTextTask,  "Hello World and all others too")
            ,guiAddSimpleButton_Color(fitMode, 100, 100, (Vec3f){0.9f, 0.2f, 0.3f}, changeButtonTextTask,  "Hello World and all others too")
        )
    );

    addChildElements(guiRoot,
        addChildElements(
            createElement((ElementSettings){
                .name = "Table",
                .pos = (Vec2i){100, 100},
                .padding = (Padding){10, 10, 10, 10},
                .width = 100,
                .height = 200,
                .texture = &tex,
                .onHover = defaultHoverFun,
                .draggable = true,
                .childGap = 10,
                .posMode = POS_ABSOLUTE,
            }),
            createElement((ElementSettings){
                .texture = pointerSchematic,
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
                    .draggable = true,
                    .texture = pointerSchematic,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = hoverAndDragFun,
                    .onClick = runTaskFun,
                    .texture = pointerSchematic,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = hoverAndDragFun,
                    .onClick = runTaskFun,
                    .texture = pointerSchematic,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = hoverAndDragFun,
                    .onClick = runTaskFun,
                    .texture = pointerSchematic,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = hoverAndDragFun,
                    .onClick = runTaskFun,
                    .texture = pointerSchematic,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = hoverAndDragFun,
                    .onClick = runTaskFun,
                    .texture = pointerSchematic,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = hoverAndDragFun,
                    .onClick = runTaskFun,
                    .texture = pointerSchematic,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                }),
                createElement((ElementSettings){
                    .onHover = hoverAndDragFun,
                    .onClick = runTaskFun,
                    .texture = pointerSchematic,
                    .width = 100,
                    .height = 20,
                    .padding = (Padding){4, 4, 4, 4},
                    .text = "hoosaasas"
                })
            )
        )
    );
}
