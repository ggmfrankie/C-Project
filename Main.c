
#include <stdio.h>

#include "Render/Engine.h"
#include "Utils/Map.h"
#include "Utils/String.h"
#include "Extern/Informatik/Spannungsteiler_A3.h"
#include "Extern/Informatik/DrunkenBishop.h"
#include "Extern/Informatik/Geldautomat.h"
#include "Extern/Informatik/MonteCarlo.h"
#include "Extern/Informatik/PointerFun.h"
#include "Extern/Informatik/Namensliste.h"

void* workerThread(void* args);

#include <windows.h>

void mainFun() {

    do {
        const Task t = popTask();
        t.func(t.userdata);
    } while (1);
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    pthread_t workerThreadID;
    pthread_create(&workerThreadID, NULL, workerThread, NULL);
    monteCarlo();
    //startEngine();
    return 0;
}

void* workerThread(void* args) {
    pthread_mutex_lock(&guiMutex);
    while (!guiInitialized) {
        pthread_cond_wait(&guiInitCond, &guiMutex);
    }
    pthread_mutex_unlock(&guiMutex);
    mainFun();
    return NULL;
}

