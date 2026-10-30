//
// Created by Stefan on 30.10.2026.
//

#pragma once
#include <ranges>

#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/TextureLocation.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Texture.hpp"

namespace ggm::TextureLoader {
    Texture loadTexture(const TextureLocation& location);

    template<typename... Components>
    requires ContainsType<TextureLocation, Components...> && ContainsType<Texture, Components...>
    void load(ComponentRegistry<Components...>& registry) {
        std::vector<std::pair<Entity, Texture>> entitiesBuffer {};

        for (auto& [count, entities, textureLocations] : registry.template getArchetypes<TextureLocation>()) {

            for (auto [entity, textureLocation] : std::views::zip(entities, textureLocations)) {
                entitiesBuffer.emplace_back(
                    entity,
                    loadTexture(textureLocation)
                );
            }
        }

        for (auto&[entity, texture] : entitiesBuffer) {
            registry.changeComponents(entity, TypeList<TextureLocation>{}, std::move(texture));
        }
    }
}
