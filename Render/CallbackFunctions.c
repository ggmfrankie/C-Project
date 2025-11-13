//
// Created by Stefan on 12.11.2025.
//

#include "CallbackFunctions.h"

#include "Render.h"

inline void callback1(Element *element, Renderer *renderer) {
    element->state = 1;
}
inline void callback2(Element *element, Renderer *renderer) {
    element->state = 2;
}

void dragFunction(Element *element, Renderer *renderer) {
    static Vec2f offset;
    static bool dragging = false;
    const bool isMouseDown = isMousePressed(renderer->window, GLFW_MOUSE_BUTTON_LEFT);

    if (!isMouseDown) {
        dragging = false;
        return;
    }

    if (glfwGetKey(renderer->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        if (!dragging) {
            offset.x = renderer->mousePos.x - element->pos.x;
            offset.y = renderer->mousePos.y - element->pos.y;
            dragging = true;
        } else {
            element->pos.x = renderer->mousePos.x - offset.x;
            element->pos.y = renderer->mousePos.y - offset.y;
        }
    } else {

    }
}