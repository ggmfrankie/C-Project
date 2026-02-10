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

    g_Renderer = newRenderer(2048, 1024, "Huhu", "ARIAL.TTF");

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

    int num = 0;
    while (!glfwWindowShouldClose(g_Renderer.window)) {
            const u_int64 startTime = now_ns();

        glfwPollEvents();

            const u_int64 pollEventsTime = now_ns() - startTime;

            const u_int64 updateLayoutTimeStart = now_ns();

        updateLayout3(&g_Renderer.guiRoot, (Vec2i){0, 0}, (Vec2i){0, 0}, &g_Renderer.font);

            const u_int64 updateLayoutTime = now_ns() - updateLayoutTimeStart;

            const u_int64 updateStateTimeStart = now_ns();

        updateState(&g_Renderer);

            const u_int64 updateStateTime = now_ns() - updateStateTimeStart;

            const u_int64 renderTimeStart = now_ns();

        pthread_mutex_lock(&guiMutex);
        g_Renderer.render(&g_Renderer);
        pthread_mutex_unlock(&guiMutex);

            const u_int64 renderTime = now_ns() - renderTimeStart;

        if (num++ > 100) {
            printf("Elapsed Time: %lf ms\n Poll: %llu, Update Layout: %llu, Update State: %llu, Render: %llu\n",(double)(now_ns()-startTime)/1000000, pollEventsTime, updateLayoutTime, updateStateTime, renderTime);
            num = 0;
        }

        Sleep(1);
    }
    glfwTerminate();
}

static Element* focusedElement = NULL;
static Element* mouseCapturedElement = NULL;
static bool dragging = false;

bool dragElement(const Renderer *renderer) {
    if (!mouseCapturedElement) return false;
    if (!mouseCapturedElement->flags.bits.draggable) return false;

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
    renderer->guiRoot.width = renderer->screenWidth;
    renderer->guiRoot.height = renderer->screenHeight;

    dragElement(renderer);
    const bool consumed = updateStateRecursively(&renderer->guiRoot, renderer);

    if (click(renderer->window, GLFW_MOUSE_BUTTON_LEFT) && !consumed) focusedElement = NULL;
    if (focusedElement && focusedElement->whileSelected) focusedElement->whileSelected(focusedElement);
}

bool updateStateRecursively(Element *element, Renderer *renderer) {
    if (element == NULL || !element->flags.bits.isActive) return false;
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
            puts("here");
            str_appendCharAt(&tfd->text, (char) codepoint, tfd->cursor.byteIndex++);

            setText_noLock(focusedElement, tfd->text.content);
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (focusedElement == NULL || focusedElement->type == t_defaultElement) return;

    if (focusedElement->type == t_textField) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            TextFieldData* tfd = focusedElement->elementData;
            if (key == GLFW_KEY_BACKSPACE && tfd->cursor.byteIndex != 0) {
                str_popCharAt(&tfd->text, --tfd->cursor.byteIndex);
                setText_noLock(focusedElement,  tfd->text.content);
            }
            else if (key == GLFW_KEY_LEFT && tfd->cursor.byteIndex != 0) {
                tfd->cursor.byteIndex--;
            }
            else if (key == GLFW_KEY_RIGHT && tfd->cursor.byteIndex < tfd->text.length) {
                tfd->cursor.byteIndex++;
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

Font* getFont() {
    Font* font = &g_Renderer.font;
    return font;
}

void addShaderPrograms(Renderer *renderer) {
    OtherShaders* otherShaders = &renderer->otherShaders;
}

double graphingFunction(const double x) {
    const Spannung value = berechneSpannungsteiler(10, 40, berechneErsatzwiderstand(30, 10 * x));
    //printf("%6.4f %6.4f\n", x*10, value);

    return value;
}