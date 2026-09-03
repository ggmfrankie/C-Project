//
// Created by ertls on 04.03.2026.
//
#pragma once
#include "GuiDefines.h"
#include "../../Utils/Math/Vector.h"

typedef struct GuiVertex {
    Vec2f pos;
    Vec2f uv;
    int bufferBinding;
    int ID;
} GuiVertex;

typedef struct ElementInstanceData {
    Vec4f color;
    Vec2f worldPos;
    int atlasID;
    int _pad[1];
} ElementInstanceData;

typedef struct MeshInstanceData {
    Vec4f color;
    int ownerID;
    int atlasID;
    int _pad[2];
} MeshInstanceData;

typedef struct {
    GuiVertex* aVertices;
    int* aIndices;
    ElementInstanceData* aElementData;
    MeshInstanceData* aMeshData;

    Vec2f clipStart;
    Vec2f clipEnd;
} MeshAccumulator;

typedef struct {
    MeshAccumulator* aUnfinished;
    MeshAccumulator* aDone;
} BatchAccumulator;