//
// Created by Stefan on 23.10.2026.
//

#pragma once
#include <span>

namespace ggm {
    struct Vector2f;
}

namespace ggm::Math {
    float distanceToSegment(const Vector2f& p, const Vector2f& a1, const Vector2f& a2);
    float closestDistanceToPolygon(const std::span<const Vector2f>& polygon, const Vector2f& point);

    bool isInsidePolygon(const std::span<const Vector2f>& polygon, const Vector2f& point);
}
