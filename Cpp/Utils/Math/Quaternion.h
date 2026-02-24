//
// Created by ertls on 20.02.2026.
//

#ifndef MIXEDPROJECT_QUATERNION_H
#define MIXEDPROJECT_QUATERNION_H

#include <cmath>
#include <array>

#include "Vector.h"

namespace Math {

struct Quaternion {
    float w, x, y, z;

    // -------------------------
    // Constructors
    // -------------------------
    constexpr Quaternion()
        : w(1), x(0), y(0), z(0) {}

    constexpr Quaternion(float w, float x, float y, float z)
        : w(w), x(x), y(y), z(z) {}

    // Identity quaternion
    static constexpr Quaternion Identity() {
        return Quaternion(1,0,0,0);
    }

    // -------------------------
    // From axis-angle
    // -------------------------
    static Quaternion fromAxisAngle(float ax, float ay, float az, float radians) {
        float half = radians * 0.5f;
        float s = std::sin(half);
        return Quaternion(
            std::cos(half),
            ax * s,
            ay * s,
            az * s
        );
    }

    // -------------------------
    // From Euler angles (pitch, yaw, roll)
    // NOTE: radians
    // -------------------------
    static Quaternion fromEuler(float pitch, float yaw, float roll) {
        float cy = std::cos(yaw   * 0.5f);
        float sy = std::sin(yaw   * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cr = std::cos(roll  * 0.5f);
        float sr = std::sin(roll  * 0.5f);

        Quaternion q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;
        return q;
    }

    static Quaternion fromEuler(const Vector3f& v) {
        return fromEuler(v.x, v.y, v.z);
    }

    // -------------------------
    // Length
    // -------------------------
    float length() const {
        return std::sqrt(w*w + x*x + y*y + z*z);
    }

    // -------------------------
    // Normalized quaternion
    // -------------------------
    Quaternion normalized() const {
        float len = length();
        if (len == 0) return Quaternion(1,0,0,0);
        return Quaternion(w/len, x/len, y/len, z/len);
    }

    void normalize() {
        float len = length();
        if (len == 0) { *this = Identity(); return; }
        w /= len; x /= len; y /= len; z /= len;
    }

    // -------------------------
    // Inverse quaternion
    // -------------------------
    Quaternion inverse() const {
        float lenSq = w*w + x*x + y*y + z*z;
        if (lenSq == 0) return Identity();
        return Quaternion(w/lenSq, -x/lenSq, -y/lenSq, -z/lenSq);
    }

    // -------------------------
    // Quaternion multiplication (combines rotations)
    // qTotal = this * other
    // -------------------------
    Quaternion operator*(const Quaternion& o) const {
        return Quaternion(
            w*o.w - x*o.x - y*o.y - z*o.z,
            w*o.x + x*o.w + y*o.z - z*o.y,
            w*o.y - x*o.z + y*o.w + z*o.x,
            w*o.z + x*o.y - y*o.x + z*o.w
        );
    }

    // -------------------------
    // Rotate a vector (3D)
    // -------------------------
    std::array<float,3> rotate(const std::array<float,3>& v) const {
        Quaternion qv(0, v[0], v[1], v[2]);
        Quaternion r = (*this) * qv * this->inverse();
        return { r.x, r.y, r.z };
    }

    // -------------------------
    // Convert quaternion -> 4×4 matrix (OpenGL column-major)
    // -------------------------
    std::array<float,16> toMatrix() const {
        std::array<float,16> M{};

        float xx = x*x; float yy = y*y; float zz = z*z;
        float xy = x*y; float xz = x*z; float yz = y*z;
        float wx = w*x; float wy = w*y; float wz = w*z;

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

    // -------------------------
    //Spherical Linear Interpolation (SLERP)
    // -------------------------
    static Quaternion slerp(const Quaternion& a, const Quaternion& b, float t) {
        Quaternion q1 = a.normalized();
        Quaternion q2 = b.normalized();

        float dot = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;

        // If dot < 0, take shortest path
        if (dot < 0.0f) {
            dot = -dot;
            q2 = Quaternion(-q2.w, -q2.x, -q2.y, -q2.z);
        }

        if (dot > 0.9995f) {
            // Nearly identical quaternions -> linear interpolation
            Quaternion r(
                q1.w + t*(q2.w - q1.w),
                q1.x + t*(q2.x - q1.x),
                q1.y + t*(q2.y - q1.y),
                q1.z + t*(q2.z - q1.z)
            );
            return r.normalized();
        }

        float theta = std::acos(dot);
        float s1 = std::sin((1.0f - t) * theta);
        float s2 = std::sin(t * theta);
        float inv = 1.0f / std::sin(theta);

        return Quaternion(
            (q1.w * s1 + q2.w * s2) * inv,
            (q1.x * s1 + q2.x * s2) * inv,
            (q1.y * s1 + q2.y * s2) * inv,
            (q1.z * s1 + q2.z * s2) * inv
        );
    }
};

} // namespace Math


#endif //MIXEDPROJECT_QUATERNION_H