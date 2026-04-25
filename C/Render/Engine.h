//
// Created by Stefan on 10.10.2025.
//

#pragma once

#include "Utils/Vector.h"
#include "GuiTypes.h"
extern bool guiInitialized;

void startEngine(void (*generateGUI)(Element* guiRoot));
Vec2i getMousePos();
Vec2i getWindowSize();
Font* getFont();
double graphingFunction(double x);


