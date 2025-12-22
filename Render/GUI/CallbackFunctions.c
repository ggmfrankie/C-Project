//
// Created by Stefan on 12.11.2025.
//

#include "../GUI/CallbackFunctions.h"

#include "GuiElementData.h"
#include "../Render.h"

bool dragFunction(Element *element, const Renderer *renderer) {
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

bool hoverCallbackFunction(Element *element, Renderer *renderer) {
    element->state = 1;
    return dragFunction(element, renderer);
}

bool clickCallbackFunction(Element *element, Renderer *renderer) {
    if (element->task.func) pushTask(element->task.func, element->task.userdata);
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

bool sliderCallbackFunction(Element *element, Renderer *renderer) {
    SliderData* sliderDate = (SliderData*)element->ElementData;

    return false;
}