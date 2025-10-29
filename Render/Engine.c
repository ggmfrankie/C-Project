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
    Renderer_init(&renderer);
    Mesh_ListAdd(&meshes, Mesh_loadSimpleQuad());

    Element_Array_add(&renderer.elements, newElement(Mesh_ListGet_ptr(&meshes ,0), 1, newVec2f(200, 200), 100, 50));
    Element_Array_add(&renderer.elements, newElement(Mesh_ListGet_ptr(&meshes ,0), 1, newVec2f(400, 100), 200, 400));

    while (!glfwWindowShouldClose(renderer.window)) {
        Sleep(20);
        glfwPollEvents();
        updateState();
        renderer.render(&renderer);
    }
    //renderer.shader.delete(&renderer.shader); TODO
    //renderer.elements.delete(&renderer.elements); TODO
    glfwTerminate();

}

void updateState() {

}





