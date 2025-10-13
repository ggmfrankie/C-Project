//
// Created by Stefan on 10.10.2025.
//
#include "Engine.h"

#include <windows.h>

#include "Render.h"


void updateState();

void startEngine() {
    Renderer renderer;
        printf("Creating Window!\n");
    renderer.window = initWindow(640, 180, "Huhu");
        printf("Loading Shaders!\n");
    renderer.shader = createShaderProgram();
    renderer.meshes = Mesh_newList(16);
        printf("Loading Mesh!\n");
    Mesh_ListAdd(&renderer.meshes, loadSimpleMesh());
        printf("Starting main Loop!\n");
    while (!glfwWindowShouldClose(renderer.window)) {
        Sleep(20);
        glfwPollEvents();
        updateState();
        render(&renderer);
    }

    Mesh_ListFree(&renderer.meshes);
}

void updateState() {

}





