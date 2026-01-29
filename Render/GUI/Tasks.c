//
// Created by Stefan on 29.11.2025.
//

#include "Tasks.h"

#include "GuiElement.h"

pthread_mutex_t taskMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t taskCond  = PTHREAD_COND_INITIALIZER;
Task taskQueue[TASK_QUEUE_MAX];
int taskHead = 0;
int taskTail = 0;

void pushTask(const GuiTask gt, void* data) {
    pthread_mutex_lock(&taskMutex);

    taskQueue[taskTail].func = gt;
    taskQueue[taskTail].userdata = data;
    taskTail = (taskTail + 1) % TASK_QUEUE_MAX;

    pthread_cond_signal(&taskCond);
    pthread_mutex_unlock(&taskMutex);
}

Task popTask() {
    pthread_mutex_lock(&taskMutex);

    while (taskHead == taskTail) {
        pthread_cond_wait(&taskCond, &taskMutex);
    }

    const Task t = taskQueue[taskHead];
    taskHead = (taskHead + 1) % TASK_QUEUE_MAX;

    pthread_mutex_unlock(&taskMutex);
    return t;
}

void changeElementText(void* userdata) {
    Element* element = userdata;
    setText(element, "New Text here");
}