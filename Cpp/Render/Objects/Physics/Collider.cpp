//
// Created by ertls on 09.03.2026.
//

#include "Collider.hpp"
#include <cmath>

namespace Obj {
    using ggm::Vector3f, ggm::Matrix4f, ggm::Vector4f;
    Collider::Collider(const Vector3f center, const float width, const float height, const float length, const Vector3f rotation): center(center) {
        const auto R = Matrix4f::Rotation(rotation);
        localAxisX = (R * Vector4f(1, 0, 0, 0)).xyz().normalize();
        localAxisY = (R * Vector4f(0, 1, 0, 0)).xyz().normalize();
        localAxisZ = (R * Vector4f(0, 0, 1, 0)).xyz().normalize();

        float hx = width  * 0.5f;
        float hy = height * 0.5f;
        float hz = length * 0.5f;

        localHalfSides = {hx, hy, hz};
    }

    Collider::~Collider() = default;

    bool Collider::isOverlap(Collider& other) {

        const Vector3f& C1 = this->center;
        const Vector3f& C2 = other.center;

        const Vector3f distance = C2 - C1;


        const Vector3f* A[3] = { &axisX, &axisY, &axisZ };
        const Vector3f* B[3] = { &other.axisX, &other.axisY, &other.axisZ };


        auto projRadius = [&](const Collider& c, const Vector3f& axis) {
                return std::fabs(axis.dot(c.axisX)) * c.halfSides.x +
                       std::fabs(axis.dot(c.axisY)) * c.halfSides.y +
                       std::fabs(axis.dot(c.axisZ)) * c.halfSides.z;
        };

        auto testAxis = [&](const Vector3f& axis) {
            if (const float len = axis.length(); len < 1e-8f) return true;

            const Vector3f n = axis.normalize();

            const float r1 = projRadius(*this, n);
            const float r2 = projRadius(other, n);

            const float dist = std::fabs(n.dot(distance));

            return dist <= (r1 + r2);
        };


        for (auto & i : A)
            if (!testAxis(*i)) return false;

        for (auto & i : B)
            if (!testAxis(*i)) return false;

        for (auto & i : A)
            for (auto & j : B) {
                if (Vector3f axis = i->cross(*j); !testAxis(axis)) return false;
            }

        return true;

    }

    void Collider::update(const Matrix4f& M) {
        const Vector3f newX = (M * Vector4f(localAxisX, 0)).xyz();
        const Vector3f newY = (M * Vector4f(localAxisY, 0)).xyz();
        const Vector3f newZ = (M * Vector4f(localAxisZ, 0)).xyz();

        axisX = newX.normalize();
        axisY = newY.normalize();
        axisZ = newZ.normalize();

        center = (M * Vector4f(localCenter, 1)).xyz();

        halfSides = {
            localHalfSides.x * newX.length(),
            localHalfSides.y * newY.length(),
            localHalfSides.z * newZ.length()
        };
    }
} // Obj