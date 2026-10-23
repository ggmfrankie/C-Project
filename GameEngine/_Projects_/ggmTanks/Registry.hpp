//
// Created by Stefan on 20.10.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/PlayerControlled.hpp"
#include "../../Engine/Core/EntityComponentSystem/Components/Render/Sprite.hpp"
#include "Engine/Core/EntityComponentSystem/Components/SpriteLocation.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Transformation.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Mesh2D.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Texture.hpp"


namespace ggmTanks {
    using Registry = ggm::ComponentRegistry<
        ggm::Sprite,
        ggm::SpriteLocation,
        ggm::Transformation,
        ggm::PlayerControlled,
        ggm::Mesh2D,
        ggm::Texture
    >;
}
