//
// Created by Stefan on 26.10.2026.
//

#pragma once
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Mesh2D.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/MeshData2D.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/MeshDataQuad.hpp"

namespace ggm::MeshLoader2D {
    using Buffer = std::vector<std::pair<Entity, Mesh2D>>;

    void loadMeshes(Buffer& buffer, std::span<const Entity> entities, std::span<const MeshData2D> meshDatas);
    void loadMeshes(Buffer& buffer, std::span<const Entity> entities);

    template<typename... Components>
    requires ContainsType<MeshData2D, Components...> && ContainsType<Mesh2D, Components...>
    void load(ComponentRegistry<Components...>& registry) {
        static Buffer entitiesBuffer;
        static Buffer spriteBuffer;

        entitiesBuffer.clear();
        spriteBuffer.clear();

        for (auto&[count, entities, meshDatas] : registry.template getArchetypes<MeshData2D>()) {
            loadMeshes(entitiesBuffer, entities, meshDatas);
        }

        for (auto&[count, entities, _] : registry.template getArchetypes<MeshDataQuad>()) {
            loadMeshes(spriteBuffer, entities);
        }

        for (auto&[entity, mesh2D] : entitiesBuffer) {
            registry.changeComponents(entity, TypeList<MeshData2D>{}, std::move(mesh2D));
        }

        for (auto&[entity, mesh2D] : spriteBuffer) {
            registry.changeComponents(entity, TypeList<MeshDataQuad>{}, std::move(mesh2D));
        }
    }
}

