//
// Created by Stefan on 07.11.2026.
//

#include "DebugRenderer.hpp"

#include <ranges>

#include "Debug/ASSERT.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Color.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Mesh2D.hpp"
#include "Engine/Core/EntityComponentSystem/Components/Render/Texture.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Loader/MeshLoader2D.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/Loader/TextureLoader.hpp"
#include "Render/Shader/Shader.hpp"

namespace ggm::DebugRenderer {
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

    void drawPhysicsObjects(const RenderContext& context, std::span<const PhysicsObjectCircle> physicsObjects) {
        Shader& shader = getShader();

        const Mesh2D mesh = MeshLoader2D::loadNormalizedCircle();
        const Texture texture = TextureLoader::loadDefaultTexture();

        shader.bind();

        shader.setUniform("color", Color::GREEN);

        shader.setUniform("screenWidth", static_cast<float>(context.width));
        shader.setUniform("screenHeight", static_cast<float>(context.height));

        for (const auto& physicsObject : physicsObjects) {
            const Vector2f dims {physicsObject.radius, physicsObject.radius};

            shader.setUniform("screenPos", physicsObject.position);
            shader.setUniform("screenDims", dims);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.id);

            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }

        shader.unbind();
    }
}