//
// Created by ertls on 17.02.2026.
//

#include "Vector.h"

namespace Math {
    Vector3f::Vector3f(const float x, const float y, const float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3f Vector3f::operator+(const Vector3f& v) const {
        return {v.x + x, v.y + y, v.z + z};
    }

    Vector3f Vector3f::operator-(const Vector3f& v) const {
        return {x-v.x, y-v.y, z-v.z};
    }
    Vector3f Vector3f::operator*(const float factor) const {
        return {x * factor, y * factor, z * factor};
    }

    Vector3f Vector3f::dot(const Vector3f& v) const {
        return {v.x*x , v.y*y , v.z*z};
    }


    Vector2f::Vector2f() {
        x = y = 0;
    }
}



