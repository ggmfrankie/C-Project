//
// Created by ertls on 04.03.2026.
//

#pragma once
#include "GuiDefines.h"
#include "GuiTypes.h"

#define MAX_GUI_VERTICES 163840
#define MAX_GUI_INDICES 163840

void initBatchedRendering();
void uploadBatchedQuads(const GuiVertex *vertices, int vt, const int* indices, int id);
void uploadElementData(Element* element);
