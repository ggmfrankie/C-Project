//
// Created by ertls on 02.03.2026.
//

#include "Update.h"

#define UPDATE_QUEUE_MAX 64

gui_Update updateQueue[UPDATE_QUEUE_MAX];
int updateHead = 0;
int updateTail = 0;

void gui_pushUpdate(GuiUpdate gu, void* data) {
    updateQueue[updateTail].func = gu;
    updateQueue[updateTail].userdata = data;
    updateTail = (updateTail + 1) % UPDATE_QUEUE_MAX;
}
void gui_popUpdate() {
    if (updateHead == updateTail) return;
    updateQueue[updateHead].func(updateQueue[updateHead].userdata);
    updateHead = (updateHead + 1) % UPDATE_QUEUE_MAX;
}
