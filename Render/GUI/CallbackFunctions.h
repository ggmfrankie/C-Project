//
// Created by Stefan on 12.11.2025.
//

#ifndef C_CALLBACKFUNCTIONS_H
#define C_CALLBACKFUNCTIONS_H
#include "GuiElement.h"
#include "GLFW/glfw3.h"

bool dragFun(Element *element, const Renderer *renderer);
bool runTaskFun(Element *element, Renderer *renderer);
bool hoverAndDragFun(Element *element, Renderer *renderer);
bool hoverAndDragFunctionInvis(Element *element, Renderer *renderer);
bool sliderCallbackFun(Element *element, Renderer *renderer);
bool textFieldCallbackFun(Element *element, Renderer *renderer);
bool click(GLFWwindow *window, int mouseButton);
bool defaultHoverFun(Element *element, Renderer *renderer);
bool changeColorOnHoverFun(Element *element, Renderer *renderer);

void syncWithScreen(Element *element);
void updateColorRainbow(Element *element);
void displayCurrentTime(Element *element);
void incrementWidth(Element *element);
void incrementHeight(Element *element);
void shiftPosition(Element *element);

#define CHANGE_COLOR_ON_HOVER(color)\
.elementData = (color),\
.onHover = changeColorOnHoverFunction,


#endif //C_CALLBACKFUNCTIONS_H
