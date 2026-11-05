//
// Created by Stefan on 23.10.2026.
//

#pragma once
#include <span>

#include "Math/Vector.hpp"


namespace ggm::Math {
    std::pair<float, Vector2f> distanceToSegment(const Vector2f& p, const Vector2f& a1, const Vector2f& a2);
    Vector2f collisionLineWithCircle(Vector2f pos, float radius, const std::span<const Vector2f>& lineSegments, Vector2f linePos = {});
    float closestDistanceToPolygon(const std::span<const Vector2f>& polygon, const Vector2f& point);

    bool isInsidePolygon(const std::span<const Vector2f>& polygon, const Vector2f& point);
}
