//
// Created by Stefan on 12.10.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Position.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Sprite.hpp"

namespace ggm::SpriteRenderer {
    void drawSprites(const std::vector<Sprite>& sprites, const std::vector<Position>& positions);

    template<typename... Components>
    requires ContainsType<Position, Components...> && ContainsType<Sprite, Components...>
    void draw(ComponentRegistry<Components...>& registry) {
        for (auto&[count, entities, sprites, positions] : registry.template getArchetypes<Sprite, Position>()) {
            drawSprites(sprites, positions);
        }
    }
}