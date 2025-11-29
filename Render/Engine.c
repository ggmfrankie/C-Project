//
// Created by Stefan on 10.10.2025.
//
#include "Engine.h"

#include <math.h>
#include <windows.h>

#include "Render.h"
#include "GUI/CallbackFunctions.h"
#include "../Extern/Informatik/Spannungsteiler_A3.h"
#include "../Utils/TimeMeasurenments.h"

    #define WIDTH 4096
    #define HEIGHT 600

List_Element* g_Elements;
pthread_mutex_t guiMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  guiInitCond = PTHREAD_COND_INITIALIZER;
int guiInitialized = false;

void updateState(Renderer *renderer);

void startEngine() {

    static Renderer renderer;
    static List_Element elementList;
    elementList = Element_newList(4);

    g_Elements = &elementList;
    renderer = newRenderer(512, 512, "Huhu", g_Elements);

    Texture graphTexture = newEmptyTexture(WIDTH, HEIGHT);
    Texture blackButton = loadTextureFromPng("GrayBox.png");
    Texture pointerSchematic = loadTextureFromPng("Pointer Schematic.png");

    renderer.computeShader = newComputeShader(NULL, 1024);
    renderer.computeShader.texture = &graphTexture;
    renderer.computeShader.thickness = 2;

    renderer.computeShader.startX = 0.0f;
    renderer.computeShader.endX = 5.0f;

    Renderer_init(&renderer);

    guiAddSimpleRectangle(g_Elements, newVec2f(0, 0), pointerSchematic.width, pointerSchematic.height, &pointerSchematic);
    guiAddSimpleButton(g_Elements, (Vec2f){100.0f, 100.0f}, 100, 100, &blackButton, hoverCallbackFunction, clickCallbackFunction,  "Hello World and all others too");

    guiInitialized = true;
    pthread_cond_broadcast(&guiInitCond);
    pthread_mutex_unlock(&guiMutex);
    int i = 0;
    while (!glfwWindowShouldClose(renderer.window)) {
        const unsigned long long timeStart = now_ns();
        glfwPollEvents();
        updateState(&renderer);

        pthread_mutex_lock(&guiMutex);
        renderer.render(&renderer);
        pthread_mutex_unlock(&guiMutex);

        const unsigned long long elapsedTime = now_ns()-timeStart;
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
    bool isVoidLayer = true;

    for (int i = 0; i < renderer->elements->size; i++) {
        Element *element = renderer->elements->get(renderer->elements, i);

        if (element->isMouseOver == NULL) continue;

        if (element->isMouseOver(element, renderer->mousePos)) {
            isVoidLayer = false;
            if (element->onClick != NULL && click(renderer->window ,GLFW_MOUSE_BUTTON_LEFT)) if (element->onClick(element, renderer)) return;
            if (element->onHover != NULL) if (element->onHover(element, renderer)) return;
        }
    }
    if (isVoidLayer) {
        if (renderer->defaultClick != NULL) renderer->defaultClick(renderer);
    }
}

double graphingFunction(const double x) {
    const Spannung value = berechneSpannungsteiler(10, 40, berechneErsatzwiderstand(30, 10 * x));
    //printf("%6.4f %6.4f\n", x*10, value);

    return value;
}