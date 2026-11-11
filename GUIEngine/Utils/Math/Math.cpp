//
// Created by Stefan on 23.10.2026.
//

#include "Math.hpp"

#include "ggmdef.hpp"
#include "Utils/Debug/Logger.hpp"
#include "Math/Vector.hpp"

namespace ggm::Math {
    using vec2 = Vector2f;
    std::pair<float, Vector2f> distanceToSegment(const Vector2f& p, const Vector2f& a1, const Vector2f& a2) {
        const vec2 segment = a2 - a1;
        const vec2 a1p     = p  - a1;

        const float segmentLenSq = segment.lengthSquared();
        if (segmentLenSq == 0.0f) return {a1p.length(), {}};

        const float t = std::clamp(segment.dot(a1p) / segmentLenSq, 0.0f, 1.0f);
        const vec2 closestPointOnSegment = a1 + t * segment;

        return {(p - closestPointOnSegment).length(), closestPointOnSegment};
    }

    float closestDistanceToPolygon(const std::span<const Vector2f>& polygon, const Vector2f& point) {
        const u64 length = polygon.size();

        float closestDistance = std::numeric_limits<float>::infinity();

        for (int i = 0; i < length; ++i) {
            const vec2 p0 = polygon[i];
            const vec2 p1 = polygon[(i + 1) % length];

            closestDistance = std::min(closestDistance, distanceToSegment(point, p0, p1).first);
        }

        return closestDistance;
    }

    bool isInsidePolygon(const std::span<const Vector2f>& polygon, const Vector2f& point) {
        const u64 length = polygon.size();
        bool inside = false;

        for (int i = 0; i < length; ++i) {
            const vec2 p0 = polygon[i];
            const vec2 p1 = polygon[(i + 1) % length];

            if (const bool yCheck = (p0.y > point.y) != (p1.y > point.y);
                !yCheck)
                continue;
            if (const float xIntersect = (p1.x - p0.x) * (point.y - p0.y) / (p1.y - p0.y) + p0.x;
                point.x < xIntersect)
                inside = !inside;
        }
        return inside;
    }

    Vector2f collisionLineWithCircle(Vector2f pos, float radius, const std::span<const Vector2f>& lineSegments, Vector2f linePos) {
        Vector2f closestPoint {};
        float closestDistance = std::numeric_limits<float>::infinity();

        for (size_t i = 0; i + 1 < lineSegments.size(); i++) {
            const Vector2f a = lineSegments[i]     + linePos;
            const Vector2f b = lineSegments[i + 1] + linePos;

            if (auto[distance, point] = distanceToSegment(pos, a, b);
                distance < radius && distance < closestDistance)
            {
                closestPoint = point;
                closestDistance = distance;
            }
        }
        if (closestDistance >= radius) return {};
        return (pos - closestPoint).normalize() * (radius - closestDistance);
    }
}
