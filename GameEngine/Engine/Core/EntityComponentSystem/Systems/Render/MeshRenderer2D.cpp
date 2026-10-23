//
// Created by Stefan on 23.10.2026.
//

#include "MeshRenderer2D.hpp"

#include <ranges>

#include "Engine/Debug/ASSERT.hpp"
#include "Render/Shader/Shader.hpp"

namespace ggm::MeshRenderer2D {
    static Shader& getShader() {
        static Shader shader = []{
            Shader out {
                "2D/SpriteShader.vert",
                "2D/SpriteShader.frag"
            };
            ASSERT::openGlLoaded();

            out.compileAndLink();
            return out;
        }();

        return shader;
    }

    void drawMeshes(
        const RenderContext& context,
        const std::vector<Mesh2D>& meshes,
        const std::vector<Texture>& textures, const std::vector<Transformation>& transformations
    ) {
        Shader& shader = getShader();
        shader.bind();

        shader.setUniform("screenWidth", static_cast<float>(context.width));
        shader.setUniform("screenHeight", static_cast<float>(context.height));

        for (const auto& [mesh, texture, transformation] : std::views::zip(meshes, textures, transformations)) {
            shader.setUniform("screenPos", transformation.position);
            shader.setUniform("screenDims", transformation.dimensions);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.id);

            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }

        shader.unbind();

    }
}


