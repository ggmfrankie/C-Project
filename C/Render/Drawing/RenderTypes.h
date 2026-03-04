//
// Created by ertls on 04.03.2026.
//

#ifndef MIXEDPROJECT_RENDERTYPES_H
#define MIXEDPROJECT_RENDERTYPES_H
#include "Utils/Vector.h"

typedef struct {
    Vec2f pos;
    Vec2f uv;
    int ID;
    int texID;
} GuiVertex;

typedef struct {
    float brightness;
    int hasTexture;
    Vec2f pos;
    Vec4f color;
} InstanceData;

#endif //MIXEDPROJECT_RENDERTYPES_H