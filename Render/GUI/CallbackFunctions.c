//
// Created by Stefan on 12.11.2025.
//

#include "../GUI/CallbackFunctions.h"

#include "../Render.h"

void dragFunction(Element *element, Renderer *renderer) {
    element->state = 1;
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
    }
}

void hoverCallbackFunction(Element *element, Renderer *renderer) {
    dragFunction(element, renderer);
}

void clickCallbackFunction(Element *element, Renderer *renderer) {
    if (glfwGetKey(renderer->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) return;
    if (element->textElement) {
        char* newText = malloc(512);
        puts("Enter the new Name:\n");
        scanf("%511s", newText);
        Strings.delete(element->textElement->text);
        *element->textElement->text = newString_c(newText);
        free(newText);
    }
    printf("Hello\n");
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