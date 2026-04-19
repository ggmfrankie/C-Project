//
// Created by Stefan on 10.10.2025.
//

#pragma once

#include <pthread.h>

#include "Utils/Vector.h"
#include "GuiTypes.h"

extern pthread_mutex_t guiMutex;
extern pthread_cond_t  guiInitCond;
extern bool guiInitialized;

void startEngine(void (*generateGUI)(Element* guiRoot));
Vec2i getMousePos();
Vec2i getWindowSize();
Font* getFont();
double graphingFunction(double x);


