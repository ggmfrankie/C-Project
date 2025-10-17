//
// Created by Stefan on 10.10.2025.
//
#include "Engine.h"

#include <windows.h>

#include "Render.h"


void updateState();

void startEngine() {
    printf("Creating Window!\n");
    printf("Loading Shaders!\n");
    Renderer renderer = newRenderer(640, 180, "Huhu");
        printf("Loading Mesh!\n");
    renderer.meshes.add(&renderer.meshes, Mesh_loadSimpleQuad());
        printf("Starting main Loop!\n");

    while (!glfwWindowShouldClose(renderer.window)) {
        Sleep(20);
        glfwPollEvents();
        updateState();
        renderer.render(&renderer);
    }

    Mesh_ListFree(&renderer.meshes);
}

void updateState() {

}





