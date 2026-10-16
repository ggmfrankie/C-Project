//
// Created by Stefan on 14.10.2026.
//

#include "SpriteLoader.hpp"

#include "GLFW/glfw3.h"
#include "stb/stb_image.h"

namespace ggm::SpriteLoader {
    namespace fs = std::filesystem;

    const fs::path DEFAULT_PATH {fs::current_path().parent_path()/"Resources"/"Textures"};

    Sprite loadSprite(const SpriteLocation& location) {
        Sprite out {};

        int width, height, channels;

        stbi_set_flip_vertically_on_load(true);

        const fs::path fullPath = DEFAULT_PATH / location.path;
        stbi_uc *pixels = stbi_load(fullPath.string().c_str(), &width, &height, &channels, 0);

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
                     width, height, 0,
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
} // ggm