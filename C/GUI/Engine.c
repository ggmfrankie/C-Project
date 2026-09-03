//
// Created by Stefan on 10.10.2025.
//
#include "GuiDefines.h"
#include "Engine.h"
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "GuiElement/GuiElement.h"
#include "Layout/Layout.h"
#include "GLFW/glfw3.h"

#include "GuiElement/IO/Update.h"
#include "Utils/Makros/Makros.h"
#include "C/GUI/Drawing/Text.h"
#include "Utils/DataStructures/CArrayList.h"

#include "GuiInterface.h"
#include "Drawing/Render.h"
#include "GuiElement/ElementTypes/TextField.h"

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
static GuiState gGuiState;
static UserCallbacks g_Callbacks;

static void Engine_processInput(GuiState *renderer, double deltaTime);
static bool Engine_processInputRec(ElementHandle elementHandle, GuiState *renderer);
static bool Engine_handleDragElement(const GuiState *renderer);
static void Engine_resetState(ElementHandle handle);
static void gui_processDebug();

static void threadExecute() {
    do {
        const Task t = popTask();
        t.func(t.userdata);
    } while (1);
}

static void* workerThreadInit(void*) {
    pthread_mutex_lock(&guiMutex);
    while (!guiInitialized) {
        pthread_cond_wait(&guiInitCond, &guiMutex);
    }
    pthread_mutex_unlock(&guiMutex);
    threadExecute();
    return nullptr;
}

static GuiState GuiState_new(GLFWwindow* window, const int width, const int height, const char *fontFile) {
    return (GuiState){
        .guiShader = newShader("GuiRender.vert", "GuiRender.frag"),
        .window = window,
        .screenWidth = width,
        .screenHeight = height,
        .font = Text_loadFontAtlas(fontFile),
        .guiRoot = Element_new((ElementSettings){
            .invisible = true,
            .name = "GUI_ROOT",
            .canNotBeSelected = true
        }),
        .texAtlas = TextureAtlas_new(2048 , 2048)
    };
}

void gui_init(GLFWwindow* window, const int width, const int height, void (*generateGUI)(Element* guiRoot)) {
    g_mainThread = pthread_self();

    Element_init();

    gGuiState = GuiState_new(window, width, height, "Inktype-MAp2J.ttf");

    Render_init(&gGuiState);

    generateGUI(Element_get(gGuiState.guiRoot));

    Texture_loadAtlas(&gGuiState.texAtlas);

    guiInitialized = true;
    pthread_cond_broadcast(&guiInitCond);

    pthread_create(&workerThreadID, nullptr, workerThreadInit, nullptr);
}

void gui_update() {
    lock();
    static double last = 0.0;
    const double curr = glfwGetTime();
    const double deltaTime = curr - last;

    gui_popUpdate();
    Engine_resetState(gGuiState.guiRoot);
    Engine_handleDragElement(&gGuiState);
    Engine_processInput(&gGuiState, deltaTime);

    Layout_updateLayout(&gGuiState);
#if GUI_DEBUG && GUI_DEBUG_PROCESS_DEBUG
    gui_processDebug();
#endif
    last = curr;
    unlock();
}

void gui_render() {
    lock();
    Render_drawGui(&gGuiState);
    unlock();
}

void _gui_addTextures(char* first, ...) {
    assert(first != nullptr);
    va_list args;
    va_start(args, first);
    f_addTextures(&gGuiState.texAtlas, first, args);
    va_end(args);
}

void gui_setTexture(Element* e, const char* name) {
    assert(e != nullptr);
    Thread_Locked(
        if (name) {
            e->visuals.texture = name;
        } else {
            e->visuals.texture = "White.png";
        }
    )
}

void gui_setActive(const char* name, const bool b) {
    assert(name != nullptr);
    Thread_Locked(
        Element_setActive_ptr(Element_getElement_ptr(name), b);
    )
}

void gui_toggleVisible(const char* name) {
    assert(name != nullptr);
    Thread_Locked(
        Element_toggleVisible_ptr(Element_getElement_ptr(name));
    )
}

void gui_setText(const char* name, const char* text) {
    assert(name != nullptr);
    assert(text != nullptr);
    Thread_Locked(
        Element_setText_ptr(Element_getElement_ptr(name), text);
    )
}

void gui_setColor(const char* name, const float r, const float g, const float b) {
    assert(name != nullptr);
    Thread_Locked(
        Element_setColor_ptr(Element_getElement_ptr(name), (Vec3f){r, g, b});
    )
}

void gui_resetColor(const char* name) {
    assert(name != nullptr);
    Thread_Locked(
        Element* e = Element_getElement_ptr(name);
        assert(e != nullptr);
        Element_setColor_ptr(e, e->visuals.defaultColor);
    )
}

void gui_setColor_ptr(Element* ptr, float r, float g, float b) {
    assert(ptr != nullptr);
    Thread_Locked(
        Element_setColor_ptr(ptr, (Vec3f){r, g, b});
    )
}

void gui_setCornerRadius(const char* name, const int radius) {
    assert(name != nullptr);
    Thread_Locked(
        Element_getElement_ptr(name)->dims.cornerRadius = radius;
    )
}

static void gui_processDebug() {
    const Element* gameBoard = Element_getElement_ptr("game board");
    const Element* parent = Element_get(gameBoard->parentElement);
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
        status = Element_getElement_ptr(name)->flags.isActive;
    )
    return status;
}

[[deprecated]]
void startEngine(void (*generateGUI)(Element* guiRoot)) {
    constexpr int width  = 512;
    constexpr int height = 512;
    gui_init(Render_initWindow(width, height, "Chess"), width, height, generateGUI);

    StandaloneTexture* graphTexture = Texture_new(WIDTH, HEIGHT);
    gGuiState.computeShader = ComputeShader_new(nullptr, 1024);
    gGuiState.computeShader.texture = graphTexture;
    gGuiState.computeShader.thickness = 2;

    gGuiState.computeShader.startX = 0.0f;
    gGuiState.computeShader.endX = 5.0f;

    //initSockets();

    glfwSetFramebufferSizeCallback(gGuiState.window, gui_resizeCallback);
    glfwSetCursorPosCallback(gGuiState.window, gui_cursorPositionCallback);

    glfwSetCharCallback(gGuiState.window, gui_charCallback);
    glfwSetKeyCallback(gGuiState.window, gui_keyCallback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(gGuiState.window)) {

        glfwPollEvents();

        gui_update();

        gui_render();

        nanosleep(&(struct timespec){.tv_sec = 0, .tv_nsec = 1000000000L}, nullptr);
    }
    glfwTerminate();
}

static Element* focusedElement = nullptr;
static Element* mouseCapturedElement = nullptr;
static bool dragging = false;

static bool Engine_handleDragElement(const GuiState *renderer) {
    if (!mouseCapturedElement) return false;
    if (!mouseCapturedElement->callbacks.requestMove) return false;

    Element* element = mouseCapturedElement;
    static Vec2f offset;

    if (!glfwGetMouseButton(renderer->window, GLFW_MOUSE_BUTTON_LEFT)) {
        dragging = false;
        mouseCapturedElement = nullptr;
        return false;
    }

    const Element *parent = Element_get(element->parentElement);
    const Vec2f parentWorldPos = parent ? parent->dims.worldPos : (Vec2f){0, 0};

    if (!dragging) {
        offset.x = renderer->mousePos.x - element->dims.worldPos.x;
        offset.y = renderer->mousePos.y - element->dims.worldPos.y;
        dragging = true;
    }

    const Vec2f newPos = {
        .x = (renderer->mousePos.x - parentWorldPos.x) - offset.x,
        .y = (renderer->mousePos.y - parentWorldPos.y) - offset.y
    };
    element->callbacks.requestMove(element, newPos);

#if GUI_DEBUG_TRACE_DRAGGING
    only_every_do(100, {
                  printf("World pos is: %f, %f, Relative pos is: %f, %f\n",
                      element->dims.worldPos.x,
                      element->dims.worldPos.y,
                      element->dims.pos.x,
                      element->dims.pos.y
                  );
                  });
#endif
    return true;
}

static bool click(GLFWwindow *window, const int mouseButton) {
    static bool wasClicked = false;
    const bool isMouseDown = glfwGetMouseButton(window, mouseButton);

    if (!isMouseDown) {
        wasClicked = false;
        return false;
    }
    if (!wasClicked) {
        wasClicked = true;
        return true;
    }
    return false;
}

static void Engine_resetState(ElementHandle handle) {
    Element* self = Element_get(handle);
    self->state = UI_STATE_NORMAL;
    for_eachRevArr(const child, self->aFlowElements,{Engine_resetState(*child);});
    for_eachRevArr(const child, self->aStaticElements,{Engine_resetState(*child);});
}

static bool Engine_processInputRoot(GuiState *renderer) {
    Element* root = Element_get(renderer->guiRoot);

    root->dims.width = renderer->screenWidth;
    root->dims.height = renderer->screenHeight;

    for_eachRevArr(const child, root->aFlowElements,
        //return if input was consumed by child element
        if (Engine_processInputRec(*child, renderer)) return true;
    );
    for_eachRevArr(const child, root->aStaticElements,
        //return if input was consumed by child element
        if (Engine_processInputRec(*child, renderer)) return true;
    );
    return false;
}

static void Engine_processInput(GuiState *renderer, double deltaTime) {
    const bool consumed = Engine_processInputRoot(renderer);

    if (click(renderer->window, GLFW_MOUSE_BUTTON_LEFT) && !consumed) focusedElement = nullptr;

    else if (focusedElement) {
        if (focusedElement->callbacks.whileSelected) focusedElement->callbacks.whileSelected(focusedElement, deltaTime);
        focusedElement->state = UI_STATE_SELECTED;
    }
}

static bool Engine_processInputRec(ElementHandle elementHandle, GuiState *renderer) {
    Element* element = Element_get(elementHandle);
    if (element == nullptr || !element->flags.isActive) return false;
    if (element->callbacks.onUpdate) element->callbacks.onUpdate(element);

    for_eachRevArr(const child, element->aFlowElements,
        //return if input was consumed by child element
        if (Engine_processInputRec(*child, renderer)) return true;
    );
    for_eachRevArr(const child, element->aStaticElements,
        //return if input was consumed by child element
        if (Engine_processInputRec(*child, renderer)) return true;
    );

    if (dragging) return false;
    if (element->callbacks.isMouseOver && element->callbacks.isMouseOver(element, renderer->mousePos)) {
        if (element->flags.canBeHovered) {
            element->state = UI_STATE_HOVER;
            if (element->callbacks.onHover && element->callbacks.onHover(element)) return true;
        }
        if (click(renderer->window, GLFW_MOUSE_BUTTON_LEFT)) {
            mouseCapturedElement = element;
            focusedElement = element;
            if (element->callbacks.onClick && element->callbacks.onClick(element)) return true;
        }
        return true;
    }
    return false;
}

void gui_charCallback(GLFWwindow* window, const unsigned int codepoint) {
    if (focusedElement == nullptr || focusedElement->type == t_defaultElement) return;

    if (focusedElement->type == t_textField) {

        if (codepoint < 128) {
            TextField_insertCharAtCursor(focusedElement, (char) codepoint);
        }
    }
}

void gui_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (g_Callbacks.onKeyPress) { g_Callbacks.onKeyPress(key, scancode, action, mods);}
    if (focusedElement == nullptr || focusedElement->type == t_defaultElement) return;
    if (key == GLFW_KEY_ESCAPE) {
        focusedElement = nullptr;
        return;
    }

    if (focusedElement->type == t_textField) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            if (key == GLFW_KEY_BACKSPACE) {
                TextField_popChar(focusedElement);
            }
            else if (key == GLFW_KEY_LEFT) {
                TextField_moveCursorBy(focusedElement, -1);
            }
            else if (key == GLFW_KEY_RIGHT) {
                TextField_moveCursorBy(focusedElement, +1);
            }
            else if (key == GLFW_KEY_ENTER) {
                const TextFieldData* tfd = focusedElement->elementData.ptr;
                if (tfd->onEnterCallback) {
                    tfd->onEnterCallback(focusedElement);
                }
                focusedElement = nullptr;
            }
        }
    }
}

void gui_resizeCallback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
    gGuiState.screenWidth = width;
    gGuiState.screenHeight = height;
}

void gui_cursorPositionCallback(GLFWwindow* window, const double xPos, const double yPos) {
    gGuiState.mousePos.x = xPos;
    gGuiState.mousePos.y = yPos;
}

Vec2f getMousePos() {
    Vec2f mousePos;
    Thread_Locked(
        mousePos = gGuiState.mousePos;
    )
    return mousePos;
}

Vec2f getWindowSize() {
    Vec2f windowSize;
    Thread_Locked(
        windowSize = (Vec2f){gGuiState.screenWidth, gGuiState.screenHeight};
    )
    return windowSize;
}

Font *getFont() {
    Font* font;
    Thread_Locked(
        font = &gGuiState.font;
    )
    return font;
}

double graphingFunction(const double x) {
    //const Spannung value = berechneSpannungsteiler(10, 40, berechneErsatzwiderstand(30, 10 * x));
    //printf("%6.4f %6.4f\n", x*10, value);

    return 0.0;
}

static bool isMainThread() {
    return pthread_equal(pthread_self(), g_mainThread);
}