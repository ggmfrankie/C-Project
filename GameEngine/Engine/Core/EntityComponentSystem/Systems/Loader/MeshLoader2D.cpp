//
// Created by Stefan on 26.10.2026.
//

#include "MeshLoader2D.hpp"
#include <ranges>

namespace ggm::MeshLoader2D {
    static Mesh2D loadMesh(const MeshData2D& data) {
        Mesh2D out {};

        glGenVertexArrays(1, &out.VAO);
        glBindVertexArray(out.VAO);
        glGenBuffers(1, &out.vertexVBO);
        glGenBuffers(1, &out.textureVBO);

        glBindBuffer(GL_ARRAY_BUFFER, out.vertexVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.vertices.size() * 2, data.vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, out.textureVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.uvs.size() * 2, data.uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glGenBuffers(1, &out.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * data.indices.size(), data.indices.data(), GL_STATIC_DRAW);

        out.indexCount = data.indices.size();

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        return out;
    }

    std::vector<std::pair<Entity, Mesh2D>> loadMeshes(std::span<const Entity> entities, std::span<const MeshData2D> meshDatas) {
        std::vector<std::pair<Entity, Mesh2D>> out;
        out.reserve(meshDatas.size());

        for (const auto& [entity, meshData] : std::views::zip(entities, meshDatas)) {
            out.emplace_back(entity, loadMesh(meshData));
        }

        return out;
    }
}
