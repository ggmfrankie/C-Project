//
// Created by Stefan on 10.10.2025.
//
#include "Engine.h"

#include <math.h>
#include <windows.h>

#include "GUI/GuiElement.h"
#include "Render.h"
#include "GUI/CallbackFunctions.h"
#include "../Extern/Informatik/Spannungsteiler_A3.h"

#include "../Utils/TimeMeasurenments.h"

    #define WIDTH 4096
    #define HEIGHT 600


Hashmap_Element g_Hashmap;
pthread_mutex_t guiMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  guiInitCond = PTHREAD_COND_INITIALIZER;
bool guiInitialized = false;

void updateState(Renderer *renderer);
void updateState2(Renderer *renderer);
bool updateStateRecursively(Element *element, Renderer *renderer);

void startEngine(void (*generateGUI)(Element* guiRoot)) {

    static Renderer renderer;
    g_Hashmap = newHashmap_Element(512);

    renderer = newRenderer(2048, 1024, "Huhu");

    Texture* graphTexture = newEmptyTexture(WIDTH, HEIGHT);
    renderer.computeShader = newComputeShader(NULL, 1024);
    renderer.computeShader.texture = graphTexture;
    renderer.computeShader.thickness = 2;

    renderer.computeShader.startX = 0.0f;
    renderer.computeShader.endX = 5.0f;

    Renderer_init(&renderer);

    generateGUI(&renderer.guiRoot);

    guiInitialized = true;
    pthread_cond_broadcast(&guiInitCond);
    pthread_mutex_unlock(&guiMutex);
    int i = 0;
    while (!glfwWindowShouldClose(renderer.window)) {
        const u_int64 timeStart = now_ns();
        glfwPollEvents();

        if (i == 2) {
            updateLayout(&renderer.guiRoot, (Vec2i){0, 0}, &renderer, 0);
            i = 0;
        }
        i++;

        updateStateRecursively(&renderer.guiRoot, &renderer);

        pthread_mutex_lock(&guiMutex);
        renderer.render(&renderer);
        pthread_mutex_unlock(&guiMutex);

        const u_int64 elapsedTime = now_ns()-timeStart;
        Sleep(1);
    }
    glfwTerminate();
}

bool updateStateRecursively(Element *element, Renderer *renderer) {
    if (element == NULL || !element->isActive) return false;

    for (int i = 0; i < element->childElements.size; i++) {
        if (updateStateRecursively(element->childElements.content[i], renderer)) return true;
    }
    if (element->isMouseOver && element->isMouseOver(element, renderer->mousePos)) {

        if (element->onHover && element->onHover(element, renderer)) return true;
        if (click(renderer->window, GLFW_MOUSE_BUTTON_LEFT) && element->onClick && element->onClick(element, renderer)) return true;
        return true;
    }
    return false;
}

Element* getGuiRoot() {

}

void addShaderPrograms(Renderer *renderer) {
    OtherShaders* otherShaders = &renderer->otherShaders;
}

[[deprecated]]
/*
void updateState2(Renderer *renderer) {
    Element *hovered = NULL;
    for (int i = 0; i < renderer->elements->size; i++) {
        renderer->elements->get(renderer->elements, i)->state = 0;
    }

    for (int i = 0; i < renderer->elements->size; i++) {
        Element* el = renderer->elements->get(renderer->elements, i);
        if (el->isMouseOver && el->isMouseOver(el, renderer->mousePos)) {
            hovered = el;
            break;
        }
    }

    if (hovered && hovered->onHover) {
        if (hovered->onHover(hovered, renderer)) return;
    }

    if (click(renderer->window, GLFW_MOUSE_BUTTON_LEFT)) {
        if (hovered && hovered->onClick) {
            hovered->onClick(hovered, renderer);
        } else if (renderer->defaultClick) {
            renderer->defaultClick(renderer);
        }
    }
}
*/



[[deprecated]]
/*
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
*/

double graphingFunction(const double x) {
    const Spannung value = berechneSpannungsteiler(10, 40, berechneErsatzwiderstand(30, 10 * x));
    //printf("%6.4f %6.4f\n", x*10, value);

    return value;
}