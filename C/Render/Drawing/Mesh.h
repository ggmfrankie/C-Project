//
// Created by Stefan on 10.10.2025.
//

#pragma once
#include <corecrt.h>

#include "GuiDefines.h"
#include "GuiTypes.h"
#include "RenderTypes.h"
#include "Math/Vector.h"

void Mesh_generateRoundedCorner(const Element* element, GuiVertex** aVertices, int** aIndices, ssize_t id);
void Mesh_customQuad(Vec2f pos, Vec2f dims, Vec4f color, GuiVertex **aVertices, int **aIndices, MeshInstanceData** aAdditional, ssize_t
                     ownerId);
