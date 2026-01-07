//
// Created by Stefan on 29.11.2025.
//

#ifndef C_TASKS_H
#define C_TASKS_H
#include <pthread.h>
#include <stdbool.h>

#define TASK_QUEUE_MAX 128
#define THIS_ELEMENT ((void*)0)
extern pthread_mutex_t taskMutex;
extern pthread_cond_t taskCond;



typedef void (*GuiTask)(void* userData);

typedef struct {
    GuiTask func;
    void *userdata;
    bool isRunning;
} Task;

extern Task taskQueue[];
extern int taskHead;
extern int taskTail;


void pushTask(GuiTask gt, void* data);
Task popTask();

void changeElementText(void* userdata);
#endif //C_TASKS_H