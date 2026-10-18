//
// Created by Stefan on 12.10.2026.
//

#include "SpriteRenderer.hpp"

#include <ranges>

#include "Engine/Core/EntityComponentSystem/Core/RenderContext.hpp"
#include "Render/Shader/Shader.hpp"

namespace ggm::SpriteRenderer {
    static Shader& getSpriteShader() {
        static Shader shader = []{
            Shader out {
                "2D/SpriteShader.vert",
                "2D/SpriteShader.frag"
            };

            out.compileAndLink();
            return out;
        }();

        return shader;
    }

    void drawSprites(const RenderContext& context, const std::vector<Sprite>& sprites, const std::vector<Transformation>& transformations) {
        Shader& shader = getSpriteShader();
        shader.bind();

        shader.setUniform("screenWidth", static_cast<float>(context.width));
        shader.setUniform("screenHeight", static_cast<float>(context.height));

        for (const auto& [sprite, transformation] : std::views::zip(sprites, transformations)) {
            shader.setUniform("screenPos", transformation.position);
            shader.setUniform("screenDims", transformation.dimensions);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, sprite.texture.mTextureId);

            glBindVertexArray(sprite.quad.VAO);
            glDrawElements(GL_TRIANGLES, sprite.quad.indexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }

        shader.unbind();
    }
}
