//
// Created by Stefan on 12.11.2025.
//

#include "../GUI/CallbackFunctions.h"

#include "GuiElementData.h"
#include "../Render.h"
#include "../../Utils/TimeMeasurenments.h"
#include "../../Utils/UtilityFun.h"
#include <windows.h>

#include "../Engine.h"

bool dragFun(Element *element, const Renderer *renderer) {
    static Vec2i offset;
    static bool dragging = false;
    const bool isMouseDown = isMousePressed(renderer->window, GLFW_MOUSE_BUTTON_LEFT);

    if (!isMouseDown) {
        dragging = false;
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

bool hoverAndDragFun(Element *element, Renderer *renderer) {
    element->state = 1;
    return dragFun(element, renderer);
}

bool hoverAndDragFunctionInvis(Element *element, Renderer *renderer) {
    return dragFun(element, renderer);
}

bool defaultHoverFun(Element *element, Renderer *renderer) {
    element->state = 1;
    return false;
}

bool changeColorOnHoverFun(Element *element, Renderer *renderer) {
    element->color = *(Vec3f*) (element->elementData);
    return false;
}

bool runTaskFun(Element *element, Renderer *renderer) {
    if (element->task.func && !element->task.isBlocked) pushTask(element->task.func, element->task.userdata);
    return true;
}

bool click(GLFWwindow *window, const int mouseButton) {
    static bool wasClicked = false;
    const bool isMouseDown = isMousePressed(window, mouseButton);

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

bool sliderCallbackFun(Element *element, Renderer *renderer) {
    SliderData* sliderDate = (SliderData*)element->elementData;

    return false;
}

bool textFieldCallbackFun(Element *element, Renderer *renderer) {
    TextFieldData* data = (TextFieldData*)(element->elementData);

}

void displayCurrentTime(Element *element) {
    static u_int64 lastTime = 0;
    if (lastTime == 0) lastTime = now_ns();

    const u_int64 currentTime = now_ns();
    const u_int64 timeNs = currentTime - lastTime;

    if ((double)timeNs * 1e-9 < 1.0) return;

    SYSTEMTIME t;
    GetLocalTime(&t);

    char time[64];

    snprintf(
        time, 64,
        "%02d:%02d:%02d\n",
           t.wHour,
           t.wMinute,
           t.wSecond);
    copyText(element, time);
}

void syncWithScreen(Element *element) {
    const Vec2i window = getWindowSize();
    element->width = window.x;
    element->height = window.y;
}

void updateColorRainbow(Element *element) {
    static double hue = 0.0f;
    static u_int64 lastTime = 0;

    if (lastTime == 0) lastTime = now_ns();

    const u_int64 currentTime = now_ns();
    const u_int64 timeNs = currentTime - lastTime;

    hue += 120.0 * (double)timeNs * 1e-9;
    if (hue >= 360.0) hue -= 360.0;

    element->color = hsv_to_rgb((float)hue, .3f, 1.0f);
    lastTime = currentTime;
}

void incrementWidth(Element *element) {
    static int calls = 0;
    static int direction = 1;
    calls++;
    if (calls < 1) return;
    calls = 0;
    element->width += direction * 1;
    if (element->width <= 0) direction = -direction;
    if (element->width >= 800) direction = -direction;
}

void incrementHeight(Element *element) {
    static int calls = 0;
    static int direction = 1;
    calls++;
    if (calls < 1) return;
    calls = 0;
    element->height += direction * 1;
    if (element->height <= 0) direction = -direction;
    if (element->height >= 800) direction = -direction;
}

void shiftPosition(Element *element) {
    static int calls = 0;
    calls++;
    if (calls < 100) return;
    calls = 0;
    element->pos.x += 20;
    element->pos.y += 20;
}