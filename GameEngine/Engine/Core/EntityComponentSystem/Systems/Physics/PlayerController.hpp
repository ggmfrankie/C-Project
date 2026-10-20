//
// Created by Stefan on 20.10.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/PlayerControlled.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Transformation.hpp"

namespace ggm::PlayerController {

    void moveEntities(std::vector<Transformation>& transformations);

    template<typename... Components>
    requires ContainsType<Transformation, Components...> && ContainsType<PlayerControlled, Components...>
    void move(ComponentRegistry<Components...>& registry) {
        for (auto&[count, entities, transformations, _] : registry.template getArchetypes<Transformation, PlayerControlled>()) {
            moveEntities(transformations);
        }
    }
}
