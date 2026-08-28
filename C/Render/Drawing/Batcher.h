//
// Created by ertls on 04.03.2026.
//

#pragma once
#include "GuiDefines.h"
#include "GuiTypes.h"

#define MAX_GUI_VERTICES 163840
#define MAX_GUI_INDICES 163840

void Batcher_init();
void uploadBatchedQuads(GuiVertex **aVertices, int **aIndices);
void uploadElementData(const Element* element);
