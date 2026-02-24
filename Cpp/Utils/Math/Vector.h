//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_VECTOR_H
#define MIXEDPROJECT_VECTOR_H
#include <cmath>

namespace Math {
    struct Vector3f {
        constexpr Vector3f(const float x, const float y, const float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Vector3f() = default;

        constexpr Vector3f operator-() const noexcept {
            return {-x, -y, -z};
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

        [[nodiscard]] constexpr Vector3f dot(const Vector3f& v) const {
            return {v.x*x , v.y*y , v.z*z};
        }

        static constexpr Vector3f normalize(const Vector3f& v) {
            const float len = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);

            if (len == 0) return Vector3f{0,0,0};

            return Vector3f{v.x/len, v.y/len, v.z/len};
        }

        [[nodiscard]] constexpr Vector3f normalize() const {
            const float len = std::sqrt(x*x + y*y + z*z);

            if (len == 0) return Vector3f{0,0,0};

            return Vector3f{x/len, y/len, z/len};
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