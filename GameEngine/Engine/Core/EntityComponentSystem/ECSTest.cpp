//
// Created by Stefan on 14.10.2026.
//

#include "ECSTest.hpp"

#include "ComponentRegistry.hpp"
#include "Components/SpriteLocation.hpp"
#include "Math/Vector.hpp"
#include "Systems/Render/SpriteLoader.hpp"

namespace ggm::ECSTest {
    void run() {

        struct Velocity : Vector2f {

        };

        struct Health {
            int amount;
        };

        ComponentRegistry<Sprite, Velocity, Health, SpriteLocation> registry{};

        registry.createEntity({}, Health{.amount = 100}, Velocity{});
        registry.createEntity({}, Health{.amount = 100}, Velocity{});
        registry.createEntity({}, Health{.amount = 100}, Velocity{});
        //registry.createEntity({}, SpriteLocation{"ERROR.png"});

        for (auto&[count, entities, velocities, healths] : registry.getArchetypes<Velocity, Health>()) {
            puts("hello");
        }


        SpriteLoader::load(registry);
    }
}
