//
// Created by Stefan on 26.10.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Mesh2D.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/MeshData2D.hpp"

namespace ggm::MeshLoader2D {
    std::vector<std::pair<Entity, Mesh2D>> loadMeshes(std::span<const Entity> entities, std::span<const MeshData2D> meshDatas);

    template<typename... Components>
    requires ContainsType<MeshData2D, Components...> && ContainsType<Mesh2D, Components...>
    void load(ComponentRegistry<Components...>& registry) {
        std::vector<std::pair<Entity, Mesh2D>> entitiesBuffer;
        for (auto&[count, entities, meshDatas] : registry.template getArchetypes<MeshData2D>()) {
            entitiesBuffer = loadMeshes(entities, meshDatas);
        }

        for (auto&[entity, mesh2D] : entitiesBuffer) {
            registry.changeComponents(entity, TypeList<MeshData2D>{}, std::move(mesh2D));
        }
    }
}

