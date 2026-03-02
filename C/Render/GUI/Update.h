//
// Created by ertls on 02.03.2026.
//

#ifndef MIXEDPROJECT_GUIUPDATETASK_H
#define MIXEDPROJECT_GUIUPDATETASK_H
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*GuiUpdate)(void* userData);

typedef struct {
    GuiUpdate func;
    void *userdata;
    bool isBlocked;
} gui_Update;

void gui_pushUpdate(GuiUpdate gt, void* data);
void gui_popUpdate();

#ifdef __cplusplus
    }
#endif
#endif //MIXEDPROJECT_GUIUPDATETASK_H