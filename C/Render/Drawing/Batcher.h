//
// Created by ertls on 04.03.2026.
//

#ifndef MIXEDPROJECT_BATCHER_H
#define MIXEDPROJECT_BATCHER_H
typedef struct Element Element;
#include "RenderTypes.h"

#define MAX_GUI_VERTICES 16384
#define MAX_GUI_INDICES 16384

void initBatchedRendering();
void uploadBatchedQuads(const GuiVertex *vertices, int vt, const int* indices, int id);
void uploadElementData(Element* element);

#endif //MIXEDPROJECT_BATCHER_H