//
// Created by Stefan on 10.10.2025.
//

#ifndef C_VECTOR_H
#define C_VECTOR_H
#include "../Utils/ArrayList.h"
    typedef struct vec3 {
        float x, y, z;
    } vec3;

    typedef struct vec2 {
        float x, y;
    } vec2;

ARRAY_LIST(vec3, vec3)
ARRAY_LIST(vec2, vec2)
#endif //C_VECTOR_H