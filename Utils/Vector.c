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

inline bool Vec3f_Equals(const Vec3f a, const Vec3f b) {
    return a.x == b.x && a.y == b.y && a.z == b.y;
}

Vec2f newVec2f(const float x, const float y) {
    return (Vec2f){
        .x = x,
        .y = y
    };
}

int Vec2i_Cross(const Vec2i v1, const Vec2i v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

inline Vec2i Vec2i_Add(const Vec2i v1, const Vec2i v2) {
    const Vec2i result = {
        v1.x + v2.x,
        v1.y + v2.y
    };
    return result;
}

inline Vec2i Vec2i_Sub(const Vec2i v1, const Vec2i v2) {
    const Vec2i result = {
        v1.x - v2.x,
        v1.y - v2.y
    };
    return result;
}

inline Vec2i Vec2i_Mul(const Vec2i v1, const int factor) {
    const Vec2i result = {
        v1.x * factor,
        v1.y * factor
    };
    return result;
}

inline Vec2f Vec2f_Sub(const Vec2f v1, const Vec2f v2) {
    const Vec2f result = {
        v1.x - v2.x,
        v1.y - v2.y
    };
    return result;
}

inline Vec2f toVec2f(const Vec2i vec) {
    return (Vec2f){
        .x = (float)vec.x,
        .y = (float)vec.y
    };
}

inline Vec3f Vec3f_Add(const Vec3f v1, const Vec3f v2) {
    const Vec3f result = {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z
    };
    return result;
}

inline Vec3f Vec3f_Mul(const Vec3f v1, const float factor) {
    const Vec3f result = {
        v1.x * factor,
        v1.y * factor,
        v1.z * factor
    };
    return result;
}

inline bool Vec2i_Equals(const Vec2i a, const Vec2i b) {
    return a.x == b.x && a.y == b.y;
}
