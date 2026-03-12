//
// Created by ertls on 09.03.2026.
//

#include "Collider.hpp"
#include <cmath>

namespace Obj {
    using ggm::Vector3f, ggm::Matrix4f, ggm::Vector4f;

    Collider::Collider(const Vector3f center, const float width, const float height, const float length, const Vector3f rotation): center(center) {
        const auto R = Matrix4f::Rotation(rotation);
        axisX = localAxisX = (R * Vector4f(1, 0, 0, 0)).xyz().normalize();
        axisY = localAxisY = (R * Vector4f(0, 1, 0, 0)).xyz().normalize();
        axisZ = localAxisZ = (R * Vector4f(0, 0, 1, 0)).xyz().normalize();

        float hx = width  * 0.5f;
        float hy = height * 0.5f;
        float hz = length * 0.5f;

        localHalfSides = {hx, hy, hz};
    }

    Collider::~Collider() = default;


    Collider::ContactInfo Collider::isOverlap(Collider& other)
    {
        ContactInfo result{};   // default: no overlap

        const Vector3f C1 = center;
        const Vector3f C2 = other.center;
        const Vector3f dist = C2 - C1;

        float minOverlap = std::numeric_limits<float>::max();
        Vector3f bestAxis{};

        const Vector3f* A[3] = { &axisX, &axisY, &axisZ };
        const Vector3f* B[3] = { &other.axisX, &other.axisY, &other.axisZ };

        auto projRadius = [](const Collider& c, const Vector3f& n){
            return
                std::fabs(n.dot(c.axisX)) * c.halfSides.x +
                std::fabs(n.dot(c.axisY)) * c.halfSides.y +
                std::fabs(n.dot(c.axisZ)) * c.halfSides.z;
        };

        auto testAxis = [&](const Vector3f& axis)
        {
            float len = axis.length();
            if (len < 1e-8f)
                return true; // skip degenerate axis

            Vector3f n = axis / len;

            float r1 = projRadius(*this, n);
            float r2 = projRadius(other, n);
            float d  = std::fabs(n.dot(dist));

            float overlap = (r1 + r2) - d;
            if (overlap < 0.0f)
                return false; // separating axis → no collision

            if (overlap < minOverlap)
            {
                minOverlap = overlap;

                // Ensure normal points from A → B
                if (n.dot(dist) < 0)
                    n = -n;

                bestAxis = n;
            }
            return true;
        };

        // --- Test A's 3 axes ---
        for (auto& ax : A)
            if (!testAxis(*ax))
                return result;

        // --- Test B's 3 axes ---
        for (auto& bx : B)
            if (!testAxis(*bx))
                return result;

        // --- Test 9 cross products ---
        for (auto& ax : A)
            for (auto& bx : B)
            {
                Vector3f axis = ax->cross(*bx);
                if (!testAxis(axis))
                    return result;
            }

        // --- SAT says overlap ---
        result.overlap = true;
        result.normal = bestAxis;
        result.penetration = minOverlap;

        // ============================
        //   Proper support point OBB
        // ============================
        auto supportPointOnOBB = [](const Collider& c, const Vector3f& dir){
                return c.center + c.axisX * (std::copysign(c.halfSides.x, dir.dot(c.axisX))) +
                c.axisY * (std::copysign(c.halfSides.y, dir.dot(c.axisY))) +
                c.axisZ * (std::copysign(c.halfSides.z, dir.dot(c.axisZ)));
        };

        Vector3f pA = supportPointOnOBB(*this,  bestAxis);
        Vector3f pB = supportPointOnOBB(other, -bestAxis);

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