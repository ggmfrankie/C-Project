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
    float transparency;
    int   hasTexture;
    float _pad0;
    Vec2f worldPos;
    float _pad1[2];
    Vec3f color;
    float _pad2;
    Vec3f textColor;
    float _pad3;
} InstanceData;

struct test {
    GuiVertex v;
    InstanceData a;
};

#endif //MIXEDPROJECT_RENDERTYPES_H