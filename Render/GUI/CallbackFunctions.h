//
// Created by Stefan on 12.11.2025.
//

#ifndef C_CALLBACKFUNCTIONS_H
#define C_CALLBACKFUNCTIONS_H
#include "GuiElement.h"
#include "GLFW/glfw3.h"

bool dragFunction(Element *element, const Renderer *renderer);
bool runTaskFunction(Element *element, Renderer *renderer);
bool hoverAndDragFunction(Element *element, Renderer *renderer);
bool sliderCallbackFunction(Element *element, Renderer *renderer);
bool click(GLFWwindow *window, int mouseButton);
bool defaultHoverFunction(Element *element, Renderer *renderer);
bool changeColorOnHoverFunction(Element *element, Renderer *renderer);

#define CHANGE_COLOR_ON_HOVER(color)\
.elementData = (color),\
.onHover = changeColorOnHoverFunction,


#endif //C_CALLBACKFUNCTIONS_H
