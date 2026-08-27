//
// Created by ertls on 20.02.2026.
//

#pragma once
#include <cmath>
#include <array>

#include "Vector.hpp"

namespace ggm {

struct Quaternion {
    float w, x, y, z;
    constexpr Quaternion()
        : w(1), x(0), y(0), z(0) {}

    constexpr Quaternion(float w, float x, float y, float z)
        : w(w), x(x), y(y), z(z) {}

    static constexpr Quaternion Identity() {
        return {1,0,0,0};
    }

    static constexpr Quaternion fromAxisAngle(float ax, float ay, float az, float radians) {
        const float half = radians * 0.5f;
        const float s = std::sin(half);
        return {
            std::cos(half),
            ax * s,
            ay * s,
            az * s
        };
    }

    static constexpr Quaternion fromEuler(float pitch, float yaw, float roll) {
        const float cy = std::cos(yaw   * 0.5f);
        const float sy = std::sin(yaw   * 0.5f);
        const float cp = std::cos(pitch * 0.5f);
        const float sp = std::sin(pitch * 0.5f);
        const float cr = std::cos(roll  * 0.5f);
        const float sr = std::sin(roll  * 0.5f);

        Quaternion q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;
        return q;
    }

    static constexpr Quaternion fromEuler(const Vector3f& v) {
        return fromEuler(v.x, v.y, v.z);
    }

    [[nodiscard]] constexpr float length() const {
        return std::sqrt(w*w + x*x + y*y + z*z);
    }

    [[nodiscard]] constexpr Quaternion normalized() const {
        const float len = length();
        if (len == 0) return {1,0,0,0};
        return {w/len, x/len, y/len, z/len};
    }

    constexpr void normalize() {
        const float len = length();
        if (len == 0) { *this = Identity(); return; }
        w /= len; x /= len; y /= len; z /= len;
    }

    [[nodiscard]] constexpr Quaternion inverse() const {
        const float lenSq = w*w + x*x + y*y + z*z;
        if (lenSq == 0) return Identity();
        return {w/lenSq, -x/lenSq, -y/lenSq, -z/lenSq};
    }

    constexpr Quaternion operator*(const Quaternion& o) const {
        return {
            w*o.w - x*o.x - y*o.y - z*o.z,
            w*o.x + x*o.w + y*o.z - z*o.y,
            w*o.y - x*o.z + y*o.w + z*o.x,
            w*o.z + x*o.y - y*o.x + z*o.w
        };
    }

    [[nodiscard]] constexpr std::array<float,3> rotate(const std::array<float,3>& v) const {
        const Quaternion qv(0, v[0], v[1], v[2]);
        const Quaternion r = (*this) * qv * this->inverse();
        return { r.x, r.y, r.z };
    }

    [[nodiscard]] constexpr std::array<float,16> toMatrix() const {
        std::array<float,16> M{};

        const float xx = x*x;
        const float yy = y*y;
        const float zz = z*z;
        const float xy = x*y;
        const float xz = x*z;
        const float yz = y*z;
        const float wx = w*x;
        const float wy = w*y;
        const float wz = w*z;

        M[0]  = 1 - 2*(yy + zz);
        M[1]  = 2*(xy + wz);
        M[2]  = 2*(xz - wy);
        M[3]  = 0;

        M[4]  = 2*(xy - wz);
        M[5]  = 1 - 2*(xx + zz);
        M[6]  = 2*(yz + wx);
        M[7]  = 0;

        M[8]  = 2*(xz + wy);
        M[9]  = 2*(yz - wx);
        M[10] = 1 - 2*(xx + yy);
        M[11] = 0;

        M[12] = 0;
        M[13] = 0;
        M[14] = 0;
        M[15] = 1;

        return M;
    }

    static constexpr Quaternion fromAngularVelocity(const Vector3f& w, float dt) {
        const float angle = w.length() * dt;

        if (angle < 1e-8f)
            return Identity();

        const Vector3f axis = w.normalize();

        const float half = angle * 0.5f;
        const float s = std::sin(half);

        return {
            std::cos(half),
            axis.x * s,
            axis.y * s,
            axis.z * s
        };
    }

    static constexpr Quaternion slerp(const Quaternion& a, const Quaternion& b, float t) {
        const Quaternion q1 = a.normalized();
        Quaternion q2 = b.normalized();

        float dot = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;

        if (dot < 0.0f) {
            dot = -dot;
            q2 = Quaternion(-q2.w, -q2.x, -q2.y, -q2.z);
        }

        if (dot > 0.9995f) {
            const Quaternion r(
                q1.w + t*(q2.w - q1.w),
                q1.x + t*(q2.x - q1.x),
                q1.y + t*(q2.y - q1.y),
                q1.z + t*(q2.z - q1.z)
            );
            return r.normalized();
        }

        const float theta = std::acos(dot);
        const float s1 = std::sin((1.0f - t) * theta);
        const float s2 = std::sin(t * theta);
        const float inv = 1.0f / std::sin(theta);

        return {
            (q1.w * s1 + q2.w * s2) * inv,
            (q1.x * s1 + q2.x * s2) * inv,
            (q1.y * s1 + q2.y * s2) * inv,
            (q1.z * s1 + q2.z * s2) * inv
        };
    }
};

} // namespace Math