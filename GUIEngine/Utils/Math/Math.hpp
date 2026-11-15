//
// Created by Stefan on 23.10.2026.
//

#pragma once
#include <span>

#include "Math/Vector.hpp"


namespace ggm::Math {
    auto distanceToSegment(
        const Vector2f& p,
        const Vector2f& a1,
        const Vector2f& a2
    ) -> std::pair<float, Vector2f>;

    auto collisionLineWithCircle(
        Vector2f pos,
        float radius,
        const std::span<const Vector2f>& lineSegments,
        Vector2f linePos = {}
    ) -> Vector2f;

    auto closestDistanceToPolygon(
        const std::span<const Vector2f>& polygon,
        const Vector2f& point
    ) -> float;

    auto isInsidePolygon(
        const std::span<const Vector2f>& polygon,
        const Vector2f& point
    ) -> bool;
}
