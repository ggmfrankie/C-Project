//
// Created by Stefan on 10.10.2025.
//

#ifndef C_ENGINE_H
#define C_ENGINE_H
#include <pthread.h>
extern pthread_mutex_t guiMutex;
extern pthread_cond_t  guiInitCond;
extern int guiInitialized;

#include "GUI/GuiElement.h"
extern List_Element* g_Elements;
    void startEngine();
    double graphingFunction(double x);
#endif //C_ENGINE_H