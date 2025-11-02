//
// Created by Stefan on 10.10.2025.
//

#ifndef C_VECTOR_H
#define C_VECTOR_H
#include "../Utils/ArrayList.h"
#include "../Utils/SimpleArray.h"
    typedef struct vec3 {
        float x, y, z;
    } Vec3f;

    typedef struct vec2 {
        float x, y;
    } Vec2f;

ARRAY_LIST(vec3, Vec3f)
ARRAY_LIST(vec2, Vec2f)

SIMPLEARRAY(vec2, Vec2f)

Vec2f Vec2f_Cross(const Vec2f* v1, const Vec2f* v2);
Vec2f newVec2f(float x, float y);

#endif //C_VECTOR_H