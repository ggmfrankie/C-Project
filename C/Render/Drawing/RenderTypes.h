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
    char _pad[8];
} GuiVertex;

typedef struct {
    float brightness;
    int hasTexture;
    Vec2f worldPos;
    Vec4f color;
    Vec3f textColor;
    char _pad[4];
} InstanceData;

struct test {
    GuiVertex v;
    InstanceData a;
};

#endif //MIXEDPROJECT_RENDERTYPES_H