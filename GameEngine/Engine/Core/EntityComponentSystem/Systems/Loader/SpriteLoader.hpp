//
// Created by Stefan on 14.10.2026.
//

#pragma once

#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "../../Components/Render/Sprite.hpp"
#include "Engine/Core/EntityComponentSystem/Components/SpriteLocation.hpp"


namespace ggm::SpriteLoader {
    Sprite loadSprite(const SpriteLocation& location);

    template<typename... Components>
    requires ContainsType<SpriteLocation, Components...> && ContainsType<Sprite, Components...>
    void load(ComponentRegistry<Components...>& registry) {
        std::vector<std::pair<Entity, Sprite>> entitiesBuffer {};
        for (auto&[count, entities, spriteLocations] : registry.template getArchetypes<SpriteLocation>()) {
            assert(entities.size() == spriteLocations.size() && entities.size() == count);

            try {
                for (int i = 0; i < count; ++i) {
                    entitiesBuffer.emplace_back(
                        entities.at(i),
                        loadSprite(spriteLocations.at(i))
                    );
                }
            } catch (std::exception& e) {
                Logger::error({"Something is not right: {}", e.what()});
            }

        }

        for (auto&[entity, sprite] : entitiesBuffer) {
            registry.changeComponents(entity, TypeList<SpriteLocation>{}, std::move(sprite));
        }
    }
}
