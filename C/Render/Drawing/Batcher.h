//
// Created by ertls on 04.03.2026.
//

#ifndef MIXEDPROJECT_BATCHER_H
#define MIXEDPROJECT_BATCHER_H
#include "Render/GUI/GuiElement.h"

void initBatchedRendering();
void renderBatchedQuads(GLuint atlasId, const GuiVertex *vertices, int length);

#endif //MIXEDPROJECT_BATCHER_H