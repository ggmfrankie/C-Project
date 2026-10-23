//
// Created by Stefan on 23.10.2026.
//

#include "Math.hpp"

#include "ggmdef.hpp"
#include "Math/Vector.hpp"

namespace ggm::Math {
    using namespace ggm;

    float distanceToSegment(const Vector2f& p, const Vector2f& a1, const Vector2f& a2) {
        using vec2 = Vector2f;

        const vec2 segment = a2 - a1;
        const vec2 a1p     = p - a1;

        const float segmentLenSq = segment.lengthSquared();
        if (segmentLenSq == 0.0f) return a1p.length();

        const float t = std::clamp(segment.dot(a1p) / segmentLenSq, 0.0f, 1.0f);
        const vec2 closestPointOnSegment = a1 + t * segment;

        return (p - closestPointOnSegment).length();
    }

    float closestDistanceToPolygon(const std::span<const Vector2f>& polygon, const Vector2f& point) {
        using namespace ggm;
        using vec2 = Vector2f;

        const u64 length = polygon.size();

        float closestDistance = std::numeric_limits<float>::infinity();

        for (int i = 0; i < length; ++i) {
            const vec2 p0 = polygon[i];
            const vec2 p1 = polygon[(i + 1) % length];

            closestDistance = std::min(closestDistance, distanceToSegment(point, p0, p1));
        }

        return closestDistance;
    }

    bool isInsidePolygon(const std::span<const Vector2f>& polygon, const Vector2f& point) {
        using namespace ggm;
        using vec2 = Vector2f;

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
}
