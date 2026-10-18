//
// Created by Stefan on 14.10.2026.
//

#include "SpriteLoader.hpp"

#include "GLFW/glfw3.h"
#include "Math/Vector.hpp"
#include "stb/stb_image.h"

namespace ggm::SpriteLoader {
    namespace fs = std::filesystem;

    Sprite::Texture loadTexture(const std::filesystem::path& path);
    Sprite::Quad loadNormalizedQuad();

    const fs::path DEFAULT_PATH {fs::current_path().parent_path()/"Resources"/"Textures"};

    Sprite loadSprite(const SpriteLocation& location) {
        const Sprite::Texture texture = loadTexture(location.path);
        static const Sprite::Quad quad = loadNormalizedQuad();

        return {texture, quad};
    }

    Sprite::Texture loadTexture(const fs::path& path) {
        Sprite::Texture out;

        int channels;

        stbi_set_flip_vertically_on_load(true);

        const fs::path fullPath = DEFAULT_PATH / path;
        stbi_uc *pixels = stbi_load(
            fullPath.string().c_str(),
            &out.width, &out.height,
            &channels,
            0
        );

        stbi_set_flip_vertically_on_load(false);

        if (!pixels) {
            std::ostringstream error;
            error << "Failed to load texture " << fullPath << "\n";
            throw std::runtime_error{error.str()};
        }

        assert(gladLoadGL(glfwGetProcAddress) != 0);

        glGenTextures(1, &out.mTextureId);
        glBindTexture(GL_TEXTURE_2D, out.mTextureId);

        glTexImage2D(GL_TEXTURE_2D, 0,
                     (channels == 4) ? GL_RGBA : GL_RGB,
                     out.width, out.height, 0,
                     (channels == 4) ? GL_RGBA : GL_RGB,
                     GL_UNSIGNED_BYTE, pixels);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(pixels);

        return out;
    }

    Sprite::Quad loadNormalizedQuad() {
        Sprite::Quad out {};

        constexpr std::array vertices {
            Vector2f{0,0},
            Vector2f{0, 1},
            Vector2f{1, 1},
            Vector2f{1, 0}
        };

        constexpr std::array uvs {
            Vector2f{0,0},
            Vector2f{0, 1},
            Vector2f{1, 1},
            Vector2f{1, 0}
        };

        constexpr std::array indices {
            0, 1, 2, 0, 2, 3
        };

        glGenVertexArrays(1, &out.VAO);
        glBindVertexArray(out.VAO);
        glGenBuffers(1, &out.vertexVBO);
        glGenBuffers(1, &out.textureVBO);

        glBindBuffer(GL_ARRAY_BUFFER, out.vertexVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 2, vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, out.textureVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvs.size() * 2, uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glGenBuffers(1, &out.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

        out.indexCount = indices.size();

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        return out;
    }
} // ggm