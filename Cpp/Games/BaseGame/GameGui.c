//
// Created by Stefan on 27.02.2026.
//

#include "GameGui.h"

#include "Projects/Test/TestGUI.h"
#include "Render/GUI/CallbackFunctions.h"


void generateGUI(Element* guiRoot) {
    addChildElements(guiRoot,
        addChildElements(
            createElement((ElementSettings){
                .name = "Home Screen",
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
                .color = {1,1,.05},
                .padding = {10,10,10,10},
                .text = "This is a test and yes Text Positioning must be refractored",
            }),
            createTextFieldElement((ElementSettings){
                .padding = {10,10,10,10},
                .height = 20,
                .width = 100,
                .color = {0.5, 1.0, 0.3}
            })
        )
    );
    setVisible(getElement("Home Screen"), false);
}
