//
// Created by Stefan on 27.02.2026.
//

#include "GameGui.h"

#include "Projects/Test/TestGUI.h"
#include "Render/GUI/CallbackFunctions.h"
#include "GuiTasks.hpp"
#include "GuiInterface.h"


void quitTask(void*) {
    puts("Bye World");
    exit(0);
}

void generateDebugGui(Element* guiRoot) {
    addChildElements(guiRoot,
        addChildElements(
            createElement((ElementSettings){
                .height = 20,
                .fixedHeight = true,
                .notSelectable = true,
                .invisible = true,
                .color = {}
            }),
            createElement((ElementSettings){
                .width = 20,
                .text = "fps",
                .notSelectable = true,
                .invisible = true,
                .name = "fps display"
            })
        )
    );
}

void generateMainMenuGui(Element* guiRoot) {
    static float fov = 70.0f;
    static float fov2 = 90.0f;
    const Task changeFov70 = {Engine_changeFOV, &fov};
    const Task changeFov90 = {Engine_changeFOV, &fov2};

    addChildElements(guiRoot,
        addChildElements(
            createElement((ElementSettings){
                .name = "Home Screen",
                .pos = (Vec2i){100, 100},
                .padding = (Padding){10, 10, 10, 10},
                .color = {0.3, 0.7, 0.9},
                .draggable = true,
                .childGap = 10,
                .posMode = POS_ABSOLUTE,
                .transparency = 0.5,
            }),
            createElement((ElementSettings){
                .invisible = true,
                .notSelectable = true,
                .text = "Home Screen",
            }),

            createTextFieldElement((ElementSettings){
                .padding = {10,10,10,10},
                .height = 20,
                .width = 100,
                .color = {0.5, 1.0, 0.3},
            }),
            createElement((ElementSettings){
                .padding = {10,10,10,10},
                .height = 20,
                .width = 100,
                .color = {0.5, 1.0, 0.3},
                .onClick = runTaskFun,
                .onHover = defaultHoverFun,
                .task = changeFov70,
                .text = "change fov to 70",
                .name = "btn_fov_70"
            }),
            createElement((ElementSettings){
                .padding = {10,10,10,10},
                .height = 20,
                .width = 100,
                .color = {0.5, 1.0, 0.3},
                .onClick = runTaskFun,
                .onHover = defaultHoverFun,
                .task = changeFov90,
                .text = "change fov to 90",
                .name = "btn_fov_90"
            }),
            createElement((ElementSettings){
                .text = "Quit",
                .padding = {10,10,10,10},
                .color = {0.5, 1.0, 0.3},
                .onHover = defaultHoverFun,
                .onClick = runTaskFun,
                .task = quitTask
            })
        )
    );

    setVisible(getElement("Home Screen"), false);
}

void generateGUI(Element* guiRoot) {
    generateDebugGui(guiRoot);
    generateMainMenuGui(guiRoot);
}
