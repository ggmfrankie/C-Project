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
#include "../Utils/Network.h"

#include "../Utils/TimeMeasurenments.h"

    #define WIDTH 4096
    #define HEIGHT 600


pthread_mutex_t guiMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  guiInitCond = PTHREAD_COND_INITIALIZER;
bool guiInitialized = false;

void updateState(Renderer *renderer);
bool updateStateRecursively(Element *element, Renderer *renderer);

void startEngine(void (*generateGUI)(Element* guiRoot)) {
    static Renderer renderer;

    renderer = newRenderer(2048, 1024, "Huhu");

    Texture* graphTexture = newEmptyTexture(WIDTH, HEIGHT);
    renderer.computeShader = newComputeShader(NULL, 1024);
    renderer.computeShader.texture = graphTexture;
    renderer.computeShader.thickness = 2;

    renderer.computeShader.startX = 0.0f;
    renderer.computeShader.endX = 5.0f;

    //initSockets();
    Renderer_init(&renderer);
    initElements();

    generateGUI(&renderer.guiRoot);

    guiInitialized = true;
    pthread_cond_broadcast(&guiInitCond);
    pthread_mutex_unlock(&guiMutex);

    while (!glfwWindowShouldClose(renderer.window)) {
        glfwPollEvents();

        updateLayout3(&renderer.guiRoot, (Vec2i){0, 0}, (Vec2i){0, 0}, &renderer.font);

        updateState(&renderer);

        pthread_mutex_lock(&guiMutex);
        renderer.render(&renderer);
        pthread_mutex_unlock(&guiMutex);

        Sleep(1);
    }
    glfwTerminate();
}

static Element* focusedElement = NULL;
static Element* mouseCapturedElement = NULL;

bool dragElement(Renderer *renderer) {
    if (!mouseCapturedElement) return false;
    if (!mouseCapturedElement->draggable) return false;

    Element* element = mouseCapturedElement;
    static Vec2i offset;
    static bool dragging = false;
    const bool isMouseDown = isMousePressed(renderer->window, GLFW_MOUSE_BUTTON_LEFT);

    if (!isMouseDown) {
        dragging = false;
        mouseCapturedElement = NULL;
        return false;
    }

    if (glfwGetKey(renderer->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        const Vec2i parentWorldPos = element->parentElement ? element->parentElement->worldPos : (Vec2i){0, 0};
        if (!dragging) {
            offset.x = renderer->mousePos.x - element->worldPos.x;
            offset.y = renderer->mousePos.y - element->worldPos.y;
            dragging = true;
        } else {
            element->positionMode = POS_ABSOLUTE;
            element->pos.x = (renderer->mousePos.x - parentWorldPos.x) - offset.x - ((element->parentElement)?element->parentElement->padding.left:0);
            element->pos.y = (renderer->mousePos.y - parentWorldPos.y) - offset.y - ((element->parentElement)?element->parentElement->padding.up:0);
        }
        return true;
    }
    return false;
}

void updateState(Renderer *renderer) {
    if (dragElement(renderer)) return;
    const bool consumed = updateStateRecursively(&renderer->guiRoot, renderer);

    if (click(renderer->window, GLFW_MOUSE_BUTTON_LEFT) && !consumed) focusedElement = NULL;
    if (focusedElement && focusedElement->whileSelected) focusedElement->whileSelected(focusedElement);
}

bool updateStateRecursively(Element *element, Renderer *renderer) {
    if (element == NULL || !element->isActive) return false;
    if (element->onUpdate) element->onUpdate(element);

    for (int i = (int)element->childElements.size-1; i >=0 ; i--) {
        if (updateStateRecursively(element->childElements.content[i], renderer)) return true;
    }
    if (element->isMouseOver && element->isMouseOver(element, renderer->mousePos)) {
        if (element->onHover && element->onHover(element, renderer)) return true;
        if (click(renderer->window, GLFW_MOUSE_BUTTON_LEFT)) {
            mouseCapturedElement = element;
            focusedElement = element;
            if (element->onClick && element->onClick(element, renderer)) return true;
        }
        return true;
    }
    return false;
}

void addShaderPrograms(Renderer *renderer) {
    OtherShaders* otherShaders = &renderer->otherShaders;
}

double graphingFunction(const double x) {
    const Spannung value = berechneSpannungsteiler(10, 40, berechneErsatzwiderstand(30, 10 * x));
    //printf("%6.4f %6.4f\n", x*10, value);

    return value;
}