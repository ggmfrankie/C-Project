//
// Created by Stefan on 10.10.2025.
//
#include "Engine.h"

#include <windows.h>

#include "GuiElement.h"
#include "Render.h"


void updateState();

void startEngine() {
    Element elementsHolder[16];
    Renderer renderer = newRenderer(640, 180, "Huhu", Element_newArray(elementsHolder, 16, 0));
    List_Mesh meshes = Mesh_newList(16);

    Mesh_ListAdd(&meshes, Mesh_loadSimpleQuad());

    while (!glfwWindowShouldClose(renderer.window)) {
        Sleep(20);
        glfwPollEvents();
        updateState();
        renderer.render(&renderer);
    }


}

void updateState() {

}





