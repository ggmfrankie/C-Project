//
// Created by Stefan on 10.10.2025.
//

#pragma once
#include "GuiDefines.h"
#include "Utils/Math/Vector.h"
#include "GuiTypes.h"
extern bool guiInitialized;

void startEngine(void (*generateGUI)(Element* guiRoot));
Vec2f getMousePos();
Vec2f getWindowSize();
Font* getFont();
double graphingFunction(double x);


