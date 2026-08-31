//
// Created by ertls on 04.03.2026.
//

#pragma once
#include "GuiDefines.h"
#include "GuiTypes.h"
#include "RenderTypes.h"
#include "Math/Vector.h"

#define MAX_GUI_VERTICES 163840
#define MAX_GUI_INDICES 163840

void Batcher_init();
void uploadMeshes(const MeshAccumulator* accumulator);
