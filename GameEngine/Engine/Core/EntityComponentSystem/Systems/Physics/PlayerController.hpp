//
// Created by Stefan on 20.10.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/PlayerControlled.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Transformation.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Physics/PhysicsObjectCircle.hpp"

namespace ggm::PlayerController {

    void moveEntities(std::span<PhysicsObjectCircle> physicsObjects);

    template<typename... Components>
    requires ContainsType<PhysicsObjectCircle, Components...> && ContainsType<PlayerControlled, Components...>
    void move(ComponentRegistry<Components...>& registry) {
        for (auto&[count, entities, physicsObjects, _] : registry.template getArchetypes<PhysicsObjectCircle, PlayerControlled>()) {
            moveEntities(physicsObjects);
        }
    }
}
