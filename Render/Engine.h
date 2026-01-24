//
// Created by Stefan on 10.10.2025.
//

#ifndef C_ENGINE_H
#define C_ENGINE_H
#include <pthread.h>
#include "GUI/GuiElement.h"



extern pthread_mutex_t guiMutex;
extern pthread_cond_t  guiInitCond;
extern bool guiInitialized;

void startEngine(void (*generateGUI)(Element* guiRoot));
Vec2i getMousePos();
Vec2i getWindowSize();
double graphingFunction(double x);


#endif //C_ENGINE_H