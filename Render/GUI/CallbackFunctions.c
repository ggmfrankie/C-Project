//
// Created by Stefan on 12.11.2025.
//

#include "../GUI/CallbackFunctions.h"

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
            offset.x = (renderer->mousePos.x - parentWorldPos.x) - (element->worldPos.x - parentWorldPos.x);
            offset.y = (renderer->mousePos.y - parentWorldPos.y) - (element->worldPos.y - parentWorldPos.y);
            dragging = true;
        } else {
            element->pos.x = (renderer->mousePos.x - parentWorldPos.x) - offset.x;
            element->pos.y = (renderer->mousePos.y - parentWorldPos.y) - offset.y;
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
    return false;
}