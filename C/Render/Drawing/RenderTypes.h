//
// Created by ertls on 04.03.2026.
//
#pragma once
#include "GuiDefines.h"
#include "Utils/Vector.h"

typedef struct GuiVertex {
    Vec2f pos;
    Vec2f uv;
    int ID;
    int texID;
    char _pad[8];
#pragma GCC poison _pad
} GuiVertex;

typedef struct InstanceData {
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
#pragma GCC poison _pad0
#pragma GCC poison _pad1
#pragma GCC poison _pad2
#pragma GCC poison _pad3
} InstanceData;

struct test {
    GuiVertex v;
    InstanceData a;
};
