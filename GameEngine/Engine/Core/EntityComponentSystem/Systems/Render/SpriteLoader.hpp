//
// Created by Stefan on 14.10.2026.
//

#pragma once
#include <iostream>

#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/SpriteLocation.hpp"

namespace ggm::SpriteLoader {
    template<typename... Components>
    requires ContainsType<SpriteLocation, Components...>
    void load(ComponentRegistry<Components...>& registry) {
        for (auto&[count, sprites] : registry.template getArchetypes<SpriteLocation>()) {
            for (int i = 0; i < count; ++i) {
                std::cout << sprites[i].path << "\n";
            }
        }
    }
}
