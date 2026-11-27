//
// Created by Stefan on 20.10.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/PlayerControlled.hpp"
#include "Components/Impact.hpp"
#include "Engine/Core/EntityComponentSystem/Components/KeyPressEvent.hpp"
#include "Engine/Core/EntityComponentSystem/Components/KeyReleaseEvent.hpp"
#include "Engine/Core/EntityComponentSystem/Components/TextureLocation.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Transformation.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Physics/PhysicsObjectCircle.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Physics/SimpleCollider.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Color.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Mesh2D.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/MeshData2D.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/MeshDataQuad.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Texture.hpp"


namespace ggmTanks {
    using Registry = ggm::ComponentRegistry<
        ggm::TextureLocation,
        ggm::Transformation,
        ggm::PlayerControlled,
        ggm::Mesh2D,
        ggm::Texture,
        ggm::MeshData2D,
        ggm::MeshDataQuad,
        ggm::SimpleCollider,
        ggm::PhysicsObjectCircle,
        ggm::Color,
        ggm::KeyPressEvent,
        ggm::KeyReleaseEvent,
        Impact
    >;
}
