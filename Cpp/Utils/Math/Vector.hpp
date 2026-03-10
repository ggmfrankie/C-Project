//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_VECTOR_H
#define MIXEDPROJECT_VECTOR_H
#include <cmath>
#include <iostream>

namespace ggm {
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

        constexpr void operator+=(const Vector3f& v) {
            x += v.x; y += v.y; z += v.z;
        }

        constexpr void operator-=(const Vector3f& v) {
            x -= v.x; y -= v.y; z -= v.z;
        }

        constexpr Vector3f operator-(const Vector3f& v) const {
            return {x-v.x, y-v.y, z-v.z};
        }

        constexpr Vector3f operator*(const float factor) const {
            return {x * factor, y * factor, z * factor};
        }

        constexpr void operator*=(const float factor){
            x *= factor; y *= factor; z *= factor;
        }

        [[nodiscard]] constexpr float dot(const Vector3f& v) const {
            return v.x*x + v.y*y + v.z*z;
        }

        [[nodiscard]] constexpr Vector3f cross(const Vector3f& v) const {
            return {
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
            };
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

        [[nodiscard]] constexpr float length() const noexcept {
            return std::sqrt(x*x + y*y + z*z);
        }

        float x;
        float y;
        float z;
    };

    inline std::ostream& operator<<(std::ostream& os, const Vector3f& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    struct Vector4f {
        constexpr Vector4f(const float x, const float y, const float z, float w):
            x(x),
            y(y),
            z(z),
            w(w)
        {}

        constexpr Vector4f(const Vector3f& v, const float w):
            x(v.x),
            y(v.y),
            z(v.z),
            w(w)
        {}

        Vector4f() = default;

        constexpr Vector4f operator-() const noexcept {
            return {-x, -y, -z, -w};
        }

        constexpr Vector4f operator+(const Vector4f& v) const {
            return {v.x + x, v.y + y, v.z + z, v.w + w};
        }

        constexpr Vector4f operator-(const Vector4f& v) const {
            return {x-v.x, y-v.y, z-v.z, w-v.w};
        }
        constexpr Vector4f operator*(const float factor) const {
            return {x * factor, y * factor, z * factor, w * factor};
        }

        [[nodiscard]] constexpr float length() const {
            return std::sqrt(x*x + y*y + z*z + w*w);
        }

        [[nodiscard]] constexpr float dot(const Vector4f& o) const {
            return o.x * o.x + o.y * o.y + o.z * o.z + o.w * w;
        }

        [[nodiscard]] constexpr Vector4f normalize() const {
            const float len = this->length();

            if (len == 0) return Vector4f{0,0,0,0};

            return Vector4f{x/len, y/len, z/len, w/len};
        }

        [[nodiscard]] constexpr Vector3f xyz() const noexcept {
            return {x, y, z};
        }

        static constexpr Vector4f normalize(const Vector4f& v) {
            const float len = v.length();

            if (len == 0) return Vector4f{0,0,0,0};

            return Vector4f{v.x/len, v.y/len, v.z/len, v.w/len};
        }

        float x;
        float y;
        float z;
        float w;
    };

    class Vector2f {
    public:
        constexpr Vector2f() = default;
        constexpr Vector2f(const float x, const float y) {
            this->x = x;
            this->y = y;
        }
        float x;
        float y;

        constexpr Vector2f operator-(const Vector2f& v) const {
            return {x-v.x, y-v.y};
        }
    };
}


#endif //MIXEDPROJECT_VECTOR_H