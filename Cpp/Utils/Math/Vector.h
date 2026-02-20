//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_VECTOR_H
#define MIXEDPROJECT_VECTOR_H

namespace Math {
    class Vector3f {
    public:
        constexpr Vector3f(const float x, const float y, const float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        constexpr Vector3f operator+(const Vector3f& v) const {
            return {v.x + x, v.y + y, v.z + z};
        }

        constexpr Vector3f operator-(const Vector3f& v) const {
            return {x-v.x, y-v.y, z-v.z};
        }
        constexpr Vector3f operator*(const float factor) const {
            return {x * factor, y * factor, z * factor};
        }

        Vector3f dot(const Vector3f& v) const {
            return {v.x*x , v.y*y , v.z*z};
        }

        float x;
        float y;
        float z;
    };

    class Vector2f {
    public:
        constexpr Vector2f(const float x, const float y) {
            this->x = x;
            this->y = y;
        }
        float x;
        float y;
    };
}


#endif //MIXEDPROJECT_VECTOR_H