//
// Created by Stefan on 05.11.2026.
//

#include "Physics2D.hpp"

#include <ranges>

#include "GUIEngine/Utils/Math/Math.hpp"

namespace ggm::Physics2D {
    static constexpr float GRAVITY = 9.81;

    void updateWithGravity(float deltaTime, std::span<Transformation> transformations, std::span<PhysicsObjectCircle> physicsObjects, std::span<const SimpleCollider> colliders) {
        for (auto[transformation, physicsObject] : std::ranges::views::zip(transformations, physicsObjects)) {
            // Sync extern position changes to the physics object
            physicsObject.position = transformation.position;

            physicsObject.position += physicsObject.velocity * deltaTime * 0.5;

            physicsObject.velocity.x += physicsObject.mass * physicsObject.acceleration.x * deltaTime;
            physicsObject.velocity.y += physicsObject.mass * (GRAVITY + physicsObject.acceleration.y) * deltaTime;

            physicsObject.position += physicsObject.velocity * deltaTime * 0.5;

            physicsObject.acceleration = {};

            for (int iteration = 0; iteration < 10; ++iteration) {
                bool collided = false;
                for (const auto& [spline, pos] : colliders) {

                    const Vector2f correction = Math::collisionLineWithCircle(physicsObject.position, physicsObject.radius, spline, pos);

                    if (correction.lengthSquared() < 0.000001f) continue;

                    collided = true;

                    Vector2f normal = correction.normalize();

                    const float normalVelocity = physicsObject.velocity.dot(normal);

                    if (normalVelocity < 0.0f) {
                        constexpr float friction = 0.9f;
                        constexpr float restitution = 0.6f;

                        Vector2f normalComponent = normal * normalVelocity;
                        Vector2f tangentComponent = physicsObject.velocity - normalComponent;

                        physicsObject.position += correction;
                        physicsObject.velocity = -restitution * normalComponent + friction * tangentComponent;
                    }
                }
                if (!collided) break;
            }

            // Sync back
            transformation.position = physicsObject.position;
        }
    }
}
