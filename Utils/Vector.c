//
// Created by Stefan on 10.10.2025.
//

#include "Vector.h"

Vec2f Vec2f_Cross(const Vec2f* v1, const Vec2f* v2) {
    return (Vec2f){
        .x = (v1->y * v2->x) - (v2->y * v1->x),
        .y = (v1->x * v2->y) - (v2->x * v1->y),
    };
}

Vec3f Vec3f_Cross(const Vec3f* v1, const Vec3f* v2) {
    return (Vec3f){
        .x = (v1->y)
    };
}

Vec2f newVec2f(const float x, const float y) {
    return (Vec2f){
        .x = x,
        .y = y
    };
}
