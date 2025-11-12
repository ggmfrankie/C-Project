//
// Created by Stefan on 10.10.2025.
//
#include "Engine.h"

#include <math.h>
#include <windows.h>

#include "GuiElement.h"
#include "Render.h"
#include "CallbackFunctions.h"
#include "../Extern/Informatik/Spannungsteiler_A3.h"
    #define width 4096
    #define height 600
void updateState(Renderer *renderer);

double graphingFunction(const double x) {
    const Spannung value = berechneSpannungsteiler(10, 40, berechneErsatzwiderstand(30, 10 * x));
    //printf("%6.4f %6.4f\n", x*10, value);

    return value;
}

void startEngine() {
    Renderer renderer = newRenderer(2048, 1024, "Huhu", Element_newList(4));
    List_Mesh meshes = Mesh_newList(16);

    Texture graphTexture = newEmptyTexture(width, height);

    renderer.computeShader = newComputeShader(NULL, 1024);
    renderer.computeShader.texture = &graphTexture;
    renderer.computeShader.thickness = 2;

    renderer.computeShader.startX = 0.0f;
    renderer.computeShader.endX = 5.0f;

    Renderer_init(&renderer);
    Mesh_ListAdd(&meshes, Mesh_loadSimpleQuad());

    Element_ListAdd(&renderer.elements, newElement(Mesh_ListGet_ptr(&meshes ,0), 1, newVec2f(100, 100), 200, 200, &graphTexture));

    Element_ListAdd(&renderer.elements, newElement(Mesh_ListGet_ptr(&meshes ,0), 1, newVec2f(200, 100), 500, 700, &graphTexture));

    setBoundingBox(Element_ListGetLast(&renderer.elements), isSelected_Quad);
    setOnHoverCallback(Element_ListGetLast(&renderer.elements), callback1);
    setOnClickCallback(Element_ListGetLast(&renderer.elements), callback2);

    while (!glfwWindowShouldClose(renderer.window)) {
        Sleep(20);
        glfwPollEvents();
        updateState(&renderer);
        renderer.render(&renderer);
    }
    //renderer.shader.delete(&renderer.shader); TODO
    //renderer.elements.delete(&renderer.elements); TODO
    glfwTerminate();
}

void updateState(Renderer *renderer) {
    for (int i = 0; i < renderer->elements.size; i++) {
        renderer->elements.get(&renderer->elements, i)->state = 0;
    }

    for (int i = 0; i < renderer->elements.size; i++) {
        Element *element = renderer->elements.get(&renderer->elements, i);

        if (element->isMouseOver == NULL) continue;
        if (element->isMouseOver(element, renderer->mousePos)) {

            if (element->onHover != NULL) element->onHover(element);

            if (element->onClick != NULL) {
                if (isMousePressed(renderer->window, GLFW_MOUSE_BUTTON_LEFT)) element->onClick(element);
            }
        }
    }
}
