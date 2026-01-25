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
#include "GUI/GuiElementData.h"

#define WIDTH 4096
    #define HEIGHT 600


pthread_mutex_t guiMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  guiInitCond = PTHREAD_COND_INITIALIZER;
bool guiInitialized = false;
Renderer g_Renderer;

void updateState(Renderer *renderer);
bool updateStateRecursively(Element *element, Renderer *renderer);
void charCallback(GLFWwindow*, unsigned int codepoint);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void startEngine(void (*generateGUI)(Element* guiRoot)) {

    g_Renderer = newRenderer(2048, 1024, "Huhu");

    Texture* graphTexture = newEmptyTexture(WIDTH, HEIGHT);
    g_Renderer.computeShader = newComputeShader(NULL, 1024);
    g_Renderer.computeShader.texture = graphTexture;
    g_Renderer.computeShader.thickness = 2;

    g_Renderer.computeShader.startX = 0.0f;
    g_Renderer.computeShader.endX = 5.0f;

    //initSockets();
    Renderer_init(&g_Renderer);
    initElements();

    glfwSetCharCallback(g_Renderer.window, charCallback);
    glfwSetKeyCallback(g_Renderer.window, keyCallback);

    generateGUI(&g_Renderer.guiRoot);

    guiInitialized = true;
    pthread_cond_broadcast(&guiInitCond);
    pthread_mutex_unlock(&guiMutex);

    while (!glfwWindowShouldClose(g_Renderer.window)) {
        glfwPollEvents();

        updateLayout3(&g_Renderer.guiRoot, (Vec2i){0, 0}, (Vec2i){0, 0}, &g_Renderer.font);

        updateState(&g_Renderer);

        pthread_mutex_lock(&guiMutex);
        g_Renderer.render(&g_Renderer);
        pthread_mutex_unlock(&guiMutex);

        Sleep(1);
    }
    glfwTerminate();
}

static Element* focusedElement = NULL;
static Element* mouseCapturedElement = NULL;
static bool dragging = false;

bool dragElement(const Renderer *renderer) {
    if (!mouseCapturedElement) return false;
    if (!mouseCapturedElement->draggable) return false;

    Element* element = mouseCapturedElement;
    static Vec2i offset;

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
    dragElement(renderer);
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
    if (dragging) return false;
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

void charCallback(GLFWwindow* window, const unsigned int codepoint) {
    if (focusedElement == NULL || focusedElement->type == t_defaultElement) return;

    if (focusedElement->type == t_textField) {
        TextFieldData* tfd = focusedElement->elementData;
        if (codepoint < 128) {
            Strings.appendChar(&tfd->text, (char) codepoint);
            tfd->cursor++;
            Strings.copyInto(&focusedElement->textElement.text, tfd->text.content);
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (focusedElement == NULL || focusedElement->type == t_defaultElement) return;

    if (focusedElement->type == t_textField) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            TextFieldData* tfd = focusedElement->elementData;
            String* text = &focusedElement->textElement.text;
            if (key == GLFW_KEY_BACKSPACE && tfd->cursor != 0)
            {
                Strings.popChar(&tfd->text);
                tfd->cursor--;
                Strings.copyInto(&focusedElement->textElement.text, tfd->text.content);
            }
            else if (key == GLFW_KEY_ENTER)
            {
                // Handle enter (e.g., submit text)
            }
        }
    }
}

Vec2i getMousePos() {
    pthread_mutex_lock(&guiMutex);
    const Vec2i mousePos = g_Renderer.mousePos;
    pthread_mutex_unlock(&guiMutex);
    return mousePos;
}

Vec2i getWindowSize() {
    pthread_mutex_lock(&guiMutex);
    const Vec2i windowSize = {g_Renderer.screenWidth, g_Renderer.screenHeight};
    pthread_mutex_unlock(&guiMutex);
    return windowSize;
}

void addShaderPrograms(Renderer *renderer) {
    OtherShaders* otherShaders = &renderer->otherShaders;
}

double graphingFunction(const double x) {
    const Spannung value = berechneSpannungsteiler(10, 40, berechneErsatzwiderstand(30, 10 * x));
    //printf("%6.4f %6.4f\n", x*10, value);

    return value;
}