//
// Created by Stefan on 10.10.2025.
//
#include "GuiDefines.h"
#include "Engine.h"

#include <windows.h>

#include "GUI/GuiElement.h"
#include "Drawing/Render.h"
#include "GUI/CallbackFunctions.h"
#include "../Extern/Informatik/Spannungsteiler_A3.h"
#include "GLFW/glfw3.h"

#include "GUI/GuiElementData.h"

#include "GUI/Update.h"
#include "Utils/Makros.h"
#include "Render/Drawing/TextDisplaying.h"
#include "Utils/CArrayList.h"

#define WIDTH 4096
#define HEIGHT 600

static pthread_mutex_t guiMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  guiInitCond = PTHREAD_COND_INITIALIZER;
static pthread_t workerThreadID;
static pthread_t g_mainThread;
static bool isMainThread();

#define Thread_Locked(...)\
    do {\
        const bool _lock = !isMainThread();\
        if (_lock) pthread_mutex_lock(&guiMutex);\
        {\
            __VA_ARGS__\
        }\
        if (_lock) pthread_mutex_unlock(&guiMutex);\
    } while (0);

#define lock() pthread_mutex_lock(&guiMutex);
#define unlock() pthread_mutex_unlock(&guiMutex);

typedef void (*GUI_onKeyPressAction)(int key, int scancode, int action, int mods);

typedef struct {
    GUI_onKeyPressAction onKeyPress;
} UserCallbacks;

bool guiInitialized = false;
static Renderer g_Renderer;
static UserCallbacks g_Callbacks;

static void processInput(Renderer *renderer);
static bool processInputRec(Element *element, Renderer *renderer);
static bool dragElement(const Renderer *renderer);
static void gui_processDebug();

static void threadExecute() {
    do {
        const Task t = popTask();
        t.func(t.userdata);
    } while (1);
}

static void* workerThreadInit(void* args) {
    pthread_mutex_lock(&guiMutex);
    while (!guiInitialized) {
        pthread_cond_wait(&guiInitCond, &guiMutex);
    }
    pthread_mutex_unlock(&guiMutex);
    threadExecute();
    return NULL;
}

void gui_init(GLFWwindow* window, const int width, const int height, void (*generateGUI)(Element* guiRoot)) {
    g_mainThread = pthread_self();
    g_Renderer = newGUIRenderer(window, width, height, "ARIAL.TTF");

    Renderer_init(&g_Renderer);

    generateGUI(g_Renderer.guiRoot);

    guiInitialized = true;
    pthread_cond_broadcast(&guiInitCond);

    pthread_create(&workerThreadID, nullptr, workerThreadInit, nullptr);
}

void gui_update() {
    lock();
    dragElement(&g_Renderer);
    Renderer_updateLayout(&g_Renderer);
    gui_popUpdate();
    processInput(&g_Renderer);
#ifdef GUI_DEBUG
    gui_processDebug();
#endif
    unlock();
}

void gui_render() {
    lock();
    Renderer_render(&g_Renderer);
    unlock();
}

void f_gui_loadTextures(char* first, ...) {
    va_list args;
    va_start(args, first);
    f_loadTextures(&g_Renderer.texAtlas, first, args);
    va_end(args);
}

void gui_setTexture(Element* e, const char* name) {
    assert(e != nullptr);
    Thread_Locked(
        if (name) {
            e->visuals.texture = getTexture(name);
            e->flags.hasTexture = true;
        } else {
            e->visuals.texture = (Texture){};
            e->flags.hasTexture = false;
        }
    )
}

void gui_setActive(const char* name, const bool b) {
    assert(name != nullptr);
    Thread_Locked(
        Element_setActive(Element_getElement(name), b);
    )
}

void gui_toggleVisible(const char* name) {
    assert(name != nullptr);
    Thread_Locked(
        Element_toggleVisible(Element_getElement(name));
    )
}

void gui_setText(const char* name, const char* text) {
    assert(name != nullptr);
    Thread_Locked(
        Element_setText(Element_getElement(name), text);
    )
}

void gui_setColor(const char* name, const float r, const float g, const float b) {
    assert(name != nullptr);
    Thread_Locked(
        Element_setColor(Element_getElement(name), (Vec3f){r, g, b});
    )
}

void gui_resetColor(const char* name) {
    assert(name != nullptr);
    Thread_Locked(
        const auto e = Element_getElement(name);
        assert(e != nullptr);
        Element_setColor(e, e->visuals.defaultColor);
    )
}

void gui_setCornerRadius(const char* name, const int radius) {
    Thread_Locked(
        Element_getElement(name)->dims.cornerRadius = radius;
    )
}

static void gui_processDebug() {
    const Element* gameBoard = Element_getElement("game board");
    const Element* parent = gameBoard->parentElement;
    assert(strcmp(parent->name, "GUI_ROOT") == 0);
}

void gui_onKeyPressCallback(GUI_onKeyPressAction action) {
    Thread_Locked(
        g_Callbacks.onKeyPress = action;
    )
}

bool gui_getActive(const char* name) {
    bool status = false;
    Thread_Locked(
        status = Element_getElement(name)->flags.isActive;
    )
    return status;
}

[[deprecated]]
void startEngine(void (*generateGUI)(Element* guiRoot)) {
    constexpr int width = 512;
    constexpr int height = 512;
    gui_init(initWindow(width, height, "Chess"), width, height, generateGUI);

    Basic_Texture* graphTexture = newEmptyTexture(WIDTH, HEIGHT);
    g_Renderer.computeShader = newComputeShader(nullptr, 1024);
    g_Renderer.computeShader.texture = graphTexture;
    g_Renderer.computeShader.thickness = 2;

    g_Renderer.computeShader.startX = 0.0f;
    g_Renderer.computeShader.endX = 5.0f;

    //initSockets();

    //glfwSetFramebufferSizeCallback(g_Renderer.window, gui_resizeCallback);
    //glfwSetCursorPosCallback(g_Renderer.window, gui_cursorPositionCallback);

    //glfwSetCharCallback(g_Renderer.window, gui_charCallback);
    //glfwSetKeyCallback(g_Renderer.window, gui_keyCallback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(g_Renderer.window)) {

        glfwPollEvents();

        gui_update();

        gui_render();

        Sleep(1);
    }
    glfwTerminate();
}

static Element* focusedElement = nullptr;
static Element* mouseCapturedElement = nullptr;
static bool dragging = false;

static bool dragElement(const Renderer *renderer) {
    if (!mouseCapturedElement) return false;
    if (!mouseCapturedElement->flags.draggable) return false;

    Element* element = mouseCapturedElement;
    static Vec2i offset;

    const bool isMouseDown = isMousePressed(renderer->window, GLFW_MOUSE_BUTTON_LEFT);

    if (!isMouseDown) {
        dragging = false;
        mouseCapturedElement = nullptr;
        return false;
    }

    if (glfwGetKey(renderer->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        const Vec2i parentWorldPos = element->parentElement ? element->parentElement->dims.worldPos : (Vec2i){0, 0};
        if (!dragging) {
            offset.x = renderer->mousePos.x - element->dims.worldPos.x;
            offset.y = renderer->mousePos.y - element->dims.worldPos.y;
            dragging = true;
        } else {
            element->positionMode = POS_ABSOLUTE;
            element->dims.pos.x = (renderer->mousePos.x - parentWorldPos.x) - offset.x - 0;
            element->dims.pos.y = (renderer->mousePos.y - parentWorldPos.y) - offset.y - 0;

#if GUI_DEBUG_TRACE_DRAGGING
            only_every_do(100, {
                printf("World pos is: %i, %i, Relative pos is: %i, %i\n",
                    element->dims.worldPos.x,
                    element->dims.worldPos.y,
                    element->dims.pos.x,
                    element->dims.pos.y
                );
            });
#endif

        }
        return true;
    }
    return false;
}

static void processInput(Renderer *renderer) {
    renderer->guiRoot->dims.width = renderer->screenWidth;
    renderer->guiRoot->dims.height = renderer->screenHeight;

    const bool consumed = processInputRec(renderer->guiRoot, renderer);

    if (click(renderer->window, GLFW_MOUSE_BUTTON_LEFT) && !consumed) focusedElement = nullptr;
    if (focusedElement && focusedElement->callbacks.whileSelected) focusedElement->callbacks.whileSelected(focusedElement);
}

static bool processInputRec(Element *element, Renderer *renderer) {
    if (element == NULL || !element->flags.isActive) return false;
    if (element->callbacks.onUpdate) element->callbacks.onUpdate(element);

    for_eachRevArr(child, element->aChildElements,
        if (processInputRec(*child, renderer)) return true;
    );

    if (dragging) return false;
    if (element->callbacks.isMouseOver && element->callbacks.isMouseOver(element, renderer->mousePos)) {
        if (element->callbacks.onHover && element->callbacks.onHover(element, renderer)) return true;
        if (click(renderer->window, GLFW_MOUSE_BUTTON_LEFT)) {
            mouseCapturedElement = element;
            focusedElement = element;
            if (element->callbacks.onClick && element->callbacks.onClick(element, renderer)) return true;
        }
        return true;
    }
    return false;
}

void gui_charCallback(GLFWwindow* window, const unsigned int codepoint) {
    if (focusedElement == nullptr || focusedElement->type == t_defaultElement) return;

    if (focusedElement->type == t_textField) {
        TextFieldData* tfd = focusedElement->elementData;
        if (codepoint < 128) {
            str_appendCharAt(&tfd->text, (char) codepoint, tfd->cursor.byteIndex++);

            Element_setText(focusedElement, tfd->text.m);
        }
    }
}

void gui_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (g_Callbacks.onKeyPress) { g_Callbacks.onKeyPress(key, scancode, action, mods);}
    if (focusedElement == NULL || focusedElement->type == t_defaultElement) return;
    if (key == GLFW_KEY_ESCAPE) {
        focusedElement = nullptr;
        return;
    }

    if (focusedElement->type == t_textField) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            TextFieldData* tfd = focusedElement->elementData;
            if (key == GLFW_KEY_BACKSPACE && tfd->cursor.byteIndex != 0) {
                str_popCharAt(&tfd->text, --tfd->cursor.byteIndex);
                Element_setText(focusedElement,  tfd->text.m);
            }
            else if (key == GLFW_KEY_LEFT && tfd->cursor.byteIndex != 0) {
                tfd->cursor.byteIndex--;
            }
            else if (key == GLFW_KEY_RIGHT && tfd->cursor.byteIndex < tfd->text.length) {
                tfd->cursor.byteIndex++;
            }
            else if (key == GLFW_KEY_ENTER) {
                if (tfd->onEnterCallback) {
                    tfd->onEnterCallback(focusedElement, &g_Renderer);
                }
                focusedElement = nullptr;
            }
        }
    }
}

void gui_resizeCallback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
    g_Renderer.screenWidth = width;
    g_Renderer.screenHeight = height;
}

void gui_cursorPositionCallback(GLFWwindow* window, const double xPos, const double yPos) {
    g_Renderer.mousePos.x = (int)xPos;
    g_Renderer.mousePos.y = (int)yPos;
}

Vec2i getMousePos() {
    Vec2i mousePos;
    Thread_Locked(
        mousePos = g_Renderer.mousePos;
    )
    return mousePos;
}

Vec2i getWindowSize() {
    const Vec2i windowSize = {g_Renderer.screenWidth, g_Renderer.screenHeight};
    return windowSize;
}

Font *getFont() {
    return &g_Renderer.font;
}

double graphingFunction(const double x) {
    const Spannung value = berechneSpannungsteiler(10, 40, berechneErsatzwiderstand(30, 10 * x));
    //printf("%6.4f %6.4f\n", x*10, value);

    return value;
}

static bool isMainThread() {
    return pthread_equal(pthread_self(), g_mainThread);
}