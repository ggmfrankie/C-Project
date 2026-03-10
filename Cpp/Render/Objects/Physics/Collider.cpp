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

    Collider::ContactInfo Collider::isOverlap(Collider& other) {

        const Vector3f& C1 = this->center;
        const Vector3f& C2 = other.center;

        float minOverlap = std::numeric_limits<float>::max();
        Vector3f bestAxis{};

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


            const float overlap = (r1 + r2) - dist;

            if (overlap < 0)
                return false;


            if (overlap < minOverlap) {
                minOverlap = overlap;

                const float sign = (distance.dot(n) < 0.0f) ? -1.0f : 1.0f;
                bestAxis = n * sign;
            }

            return true;
        };
        auto result = ContactInfo();

        for (auto& i : A)
            if (!testAxis(*i))
                return result;

        for (auto& i : B)
            if (!testAxis(*i))
                return result;

        for (auto& i : A)
            for (auto& j : B) {
                if (Vector3f axis = i->cross(*j); !testAxis(axis))
                    return result;
            }

        result.overlap = true;
        result.normal = bestAxis;
        result.penetration = minOverlap;

        // Project both OBBs onto the collision normal
        float extentA =
            fabs(bestAxis.dot(axisX)) * halfSides.x +
            fabs(bestAxis.dot(axisY)) * halfSides.y +
            fabs(bestAxis.dot(axisZ)) * halfSides.z;

        float extentB =
            fabs(bestAxis.dot(other.axisX)) * other.halfSides.x +
            fabs(bestAxis.dot(other.axisY)) * other.halfSides.y +
            fabs(bestAxis.dot(other.axisZ)) * other.halfSides.z;

        // Closest support points along normal
        Vector3f pA = center + bestAxis * extentA;
        Vector3f pB = other.center - bestAxis * extentB;

        result.point = (pA + pB) * 0.5f;


        return result;
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