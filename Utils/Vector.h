//
// Created by Stefan on 10.10.2025.
//

#ifndef C_VECTOR_H
#define C_VECTOR_H
#include <stdbool.h>

#include "../Utils/ArrayList.h"
#include "../Utils/SimpleArray.h"
    typedef struct vec3f {
        float x, y, z;
    } Vec3f;

    typedef struct vec2f {
        float x, y;
    } Vec2f;

    typedef struct vec2i {
        int x, y;
    } Vec2i;

    typedef struct vec4i {
        int x, y, z, w;
    } Vec4i;


ARRAY_LIST(vec3, Vec3f)
ARRAY_LIST(vec2, Vec2f)

SIMPLEARRAY(vec2, Vec2f)


int Vec2i_Cross(Vec2i v1, Vec2i v2);
Vec2i Vec2i_Sub(Vec2i v1, Vec2i v2);
bool Vec2i_Equals(Vec2i a, Vec2i b);

Vec2f Vec2f_Cross(const Vec2f* v1, const Vec2f* v2);
Vec2i Vec2i_Add(Vec2i v1, Vec2i v2);
Vec2f Vec2f_Sub(Vec2f v1, Vec2f v2);

Vec3f Vec3f_Add(Vec3f v1, Vec3f v2);
Vec3f Vec3f_Mul(Vec3f v1, float factor);

bool Vec3f_Equals(Vec3f a, Vec3f b);

Vec2f newVec2f(float x, float y);

Vec2f toVec2f(Vec2i vec);


#define v_sub(vec1, vec2) _Generic((vec1),\
Vec2i: Vec2i_Sub,\
Vec2f: Vec2f_Sub\
)(vec1, vec2)

#endif //C_VECTOR_H