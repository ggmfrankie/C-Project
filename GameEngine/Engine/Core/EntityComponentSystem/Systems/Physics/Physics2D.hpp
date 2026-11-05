//
// Created by Stefan on 05.11.2026.
//

#pragma once
#include <span>

#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Transformation.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Physics/PhysicsObjectCircle.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Physics/SimpleCollider.hpp"

namespace ggm::Physics2D {
    void updateWithGravity(
        float deltaTime,
        std::span<Transformation> transformations,
        std::span<PhysicsObjectCircle> physicsObjects,
        std::span<const SimpleCollider> colliders
    );

    template<typename... Components>
    requires ContainsType<Transformation, Components...> && ContainsType<PhysicsObjectCircle, Components...> && ContainsType<SimpleCollider, Components...>
    void update(ComponentRegistry<Components...>& registry, float deltaTime) {

        auto& collidersList = registry.template getArchetypes<SimpleCollider>();

        for (auto&[count, entities, transformations, physicsObjects] : registry.template getArchetypes<Transformation, PhysicsObjectCircle>()) {
            for (auto&[_, __, colliders] : collidersList) {
                updateWithGravity(deltaTime, transformations, physicsObjects, colliders);
            }
        }
    }
}
