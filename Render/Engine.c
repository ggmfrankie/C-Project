//
// Created by Stefan on 10.10.2025.
//
#include "Engine.h"

#include <math.h>
#include <windows.h>
#include <pthread.h>

#include "GUI/GuiElement.h"
#include "Render.h"
#include "GUI/CallbackFunctions.h"
#include "../Extern/Informatik/Spannungsteiler_A3.h"
#include "../Utils/TimeMeasurenments.h"

    #define width 4096
    #define height 600

List_Element* g_Elements;

void updateState(Renderer *renderer);

double graphingFunction(const double x) {
    const Spannung value = berechneSpannungsteiler(10, 40, berechneErsatzwiderstand(30, 10 * x));
    //printf("%6.4f %6.4f\n", x*10, value);

    return value;
}
#define BUTTON_CALLBACK_PRESET1 \
setBoundingBox(Element_ListGetLast(&renderer.elements), isSelected_Quad);\
setOnHoverCallback(Element_ListGetLast(&renderer.elements), dragFunction);\
setOnClickCallback(Element_ListGetLast(&renderer.elements), clickCallbackFunction);

void startEngine() {
    static Renderer renderer;
    static List_Element elementList;
    elementList = Element_newList(4);

    g_Elements = &elementList;
    renderer = newRenderer(512, 512, "Huhu", g_Elements);

    Texture graphTexture = newEmptyTexture(width, height);
    Texture blackButton = loadTextureFromPng(stringOf("GrayBox.png"));

    renderer.computeShader = newComputeShader(NULL, 1024);
    renderer.computeShader.texture = &graphTexture;
    renderer.computeShader.thickness = 2;

    renderer.computeShader.startX = 0.0f;
    renderer.computeShader.endX = 5.0f;

    Renderer_init(&renderer);

    stringOf("../Resources/Fonts/ARIAL.TTF");

    guiAddSimpleRectangle(g_Elements, newVec2f(0, 0), 200, 200, &graphTexture);
    guiAddSimpleButton(g_Elements, (Vec2f){100.0f, 100.0f}, 100, 100, &blackButton, hoverCallbackFunction, clickCallbackFunction,  "Hello World and all others too");

    int i = 0;
    while (!glfwWindowShouldClose(renderer.window)) {
        const unsigned long long timeStart = now_ns();

        glfwPollEvents();
        updateState(&renderer);
        renderer.render(&renderer);
        const unsigned long long elapsedTime = now_ns()-timeStart;
        i++;

        setText_int(g_Elements->get(g_Elements, 1), i);
        Sleep(1);
    }


    //renderer.shader.delete(&renderer.shader); TODO
    //renderer.elements.delete(&renderer.elements); TODO
    glfwTerminate();
}

void updateState(Renderer *renderer) {
    for (int i = 0; i < renderer->elements->size; i++) {
        renderer->elements->get(renderer->elements, i)->state = 0;
    }

    for (int i = 0; i < renderer->elements->size; i++) {
        Element *element = renderer->elements->get(renderer->elements, i);

        if (element->isMouseOver == NULL) continue;

        if (element->isMouseOver(element, renderer->mousePos)) {

            if (element->onClick != NULL && click(renderer->window ,GLFW_MOUSE_BUTTON_LEFT)) element->onClick(element, renderer);
            if (element->onHover != NULL) element->onHover(element, renderer);
        }
    }
}

