//
// Created by Stefan on 10.10.2025.
//
#include "Engine.h"

#include <math.h>
#include <windows.h>

#include "GuiElement.h"
#include "Render.h"
#include "../Extern/Informatik/Spannungsteiler_A3.h"
    #define width 4096
    #define height 600
void updateState();

double graphingFunction(const double x) {
    const Spannung value = berechneSpannungsteiler(10, 40, berechneErsatzwiderstand(30, 10 * x));
    //printf("%6.4f %6.4f\n", x*10, value);

    return value;
}

void startEngine() {
    Element elementsHolder[16];
    Renderer renderer = newRenderer(width+20, height+20, "Huhu", Element_newArray(elementsHolder, 16));
    List_Mesh meshes = Mesh_newList(16);

    Texture graphTexture = newEmptyTexture(width, height);

    renderer.computeShader = newComputeShader(NULL, 1024);
    renderer.computeShader.texture = &graphTexture;
    renderer.computeShader.thickness = 2;

    renderer.computeShader.startX = 0.0f;
    renderer.computeShader.endX = 5.0f;

    Renderer_init(&renderer);
    Mesh_ListAdd(&meshes, Mesh_loadSimpleQuad());

    Element_Array_set(&renderer.elements, 0,  newElement(Mesh_ListGet_ptr(&meshes ,0), 1, newVec2f(10, 10), width, height, &graphTexture));

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
