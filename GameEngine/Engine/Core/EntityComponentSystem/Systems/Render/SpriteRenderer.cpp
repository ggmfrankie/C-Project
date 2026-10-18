//
// Created by Stefan on 12.10.2026.
//

#include "SpriteRenderer.hpp"

#include "Render/Shader/Shader.hpp"

namespace ggm::SpriteRenderer {
    static Shader& getSpriteShader() {
        static Shader shader = []{
            Shader out {
                "2D/Shader2D.vert",
                "2D/Shader2D.frag"
            };

            out.compileAndLink();
            return out;
        }();

        return shader;
    }

    void drawSprites(const std::vector<Sprite>& sprites, const std::vector<Position>& positions) {
        const Shader& shader = getSpriteShader();
        shader.bind();

        for (const auto& [texture, quad] : sprites) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.mTextureId);

            glBindVertexArray(quad.VAO);
            glDrawElements(GL_TRIANGLES, quad.indexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }

        shader.unbind();
    }
}
