//
// Created by Stefan on 12.11.2025.
//

#ifndef C_CALLBACKFUNCTIONS_H
#define C_CALLBACKFUNCTIONS_H
#include "GuiElement.h"
#include "GLFW/glfw3.h"

bool dragFunction(Element *element, Renderer *renderer);
bool clickCallbackFunction(Element *element, Renderer *renderer);
bool hoverCallbackFunction(Element *element, Renderer *renderer);
bool click(GLFWwindow *window, int mouseButton);


#endif //C_CALLBACKFUNCTIONS_H
