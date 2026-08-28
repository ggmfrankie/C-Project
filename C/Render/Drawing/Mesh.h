//
// Created by Stefan on 10.10.2025.
//

#pragma once
#include "GuiDefines.h"
#include "GuiTypes.h"
#include "Math/Vector.h"

void Mesh_generateRoundedCorner(const Element* element, GuiVertex **aVertices, int **aIndices);
void Mesh_customQuad(const Element* element, Vec2f pos, Vec2f dims, GuiVertex **aVertices, int **aIndices);
