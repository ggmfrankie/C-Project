//
// Created by Stefan on 20.10.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/PlayerControlled.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Sprite.hpp"
#include "Engine/Core/EntityComponentSystem/Components/SpriteLocation.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Transformation.hpp"


namespace ggmTanks {
    using Registry = ggm::ComponentRegistry<
        ggm::Sprite,
        ggm::SpriteLocation,
        ggm::Transformation,
        ggm::PlayerControlled
    >;
}
