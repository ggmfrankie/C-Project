//
// Created by Stefan on 07.11.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Physics/PhysicsObjectCircle.hpp"
#include "Engine/Core/EntityComponentSystem/Core/RenderContext.hpp"

namespace ggm::DebugRenderer {

    void drawPhysicsObjects(
        const RenderContext& context,
        std::span<const PhysicsObjectCircle> physicsObjects
    );

    template<typename... Components>
    requires ContainsType<PhysicsObjectCircle, Components...>
    void draw(ComponentRegistry<Components...>& registry, const RenderContext& context) {
        for (auto&[count, entities, physicsObjects] : registry.template getArchetypes<PhysicsObjectCircle>()) {
            drawPhysicsObjects(context, physicsObjects);
        }
    }
}
