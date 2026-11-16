//
// Created by Stefan on 23.10.2026.
//

#include "MeshRenderer2D.hpp"

#include <ranges>
#include <sys/stat.h>

#include "Debug/Assert.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Color.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Loader/TextureLoader.hpp"
#include "Render/Shader/Shader.hpp"

namespace ggm::MeshRenderer2D {
    static Shader& getShader() {
        static Shader shader = []{
            Shader out {
                "2D/SpriteShader.vert",
                "2D/SpriteShader.frag"
            };
            Assert::openGlLoaded();

            out.compileAndLink();
            return out;
        }();

        return shader;
    }

    static Texture& getDefaultTexture() {
        static Texture texture = TextureLoader::loadTexture({"White.png"});
        return texture;
    }

    static void drawMesh(
        const Mesh2D& mesh,
        const Texture& texture,
        const Color& color,
        const Transformation& transformation
    ) {
        Shader& shader = getShader();
        shader.setUniform("color", color);

        shader.setUniform("screenPos", transformation.position);
        shader.setUniform("screenDims", transformation.dimensions);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.id);

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void drawMeshes(
        std::span<const Mesh2D> meshes,
        std::span<const Texture> textures,
        std::span<const Color> colors,
        std::span<const Transformation> transformations
    ) {
        for (auto[mesh, texture, color, transformation] : std::views::zip(meshes, textures, colors, transformations)) {
            drawMesh(mesh, texture, color, transformation);
        }
    }

    void drawMeshes(
        std::span<const Mesh2D> meshes,
        std::span<const Texture> textures,
        std::span<const Transformation> transformations
    ) {
        for (auto[mesh, texture, transformation] : std::views::zip(meshes, textures, transformations)) {
            drawMesh(mesh, texture, Color::WHITE, transformation);
        }
    }

    void drawMeshes(
        std::span<const Mesh2D> meshes,
        std::span<const Color> colors,
        std::span<const Transformation> transformations
    ) {
        for (auto[mesh, color, transformation] : std::views::zip(meshes, colors, transformations)) {
            drawMesh(mesh, getDefaultTexture(), color, transformation);
        }
    }

    void drawBegin(const RenderContext& context) {
        Shader& shader = getShader();

        shader.bind();
        shader.setUniform("screenWidth", static_cast<float>(context.width));
        shader.setUniform("screenHeight", static_cast<float>(context.height));
    }
}


