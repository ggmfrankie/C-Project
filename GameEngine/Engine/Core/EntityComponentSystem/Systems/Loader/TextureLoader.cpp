//
// Created by Stefan on 30.10.2026.
//

#include "TextureLoader.hpp"

#include "Engine/Debug/ASSERT.hpp"
#include "stb/stb_image.h"

namespace ggm::TextureLoader {
    namespace fs = std::filesystem;

    static Texture loadTexture(const std::filesystem::path& path);
    static Texture loadDefaultTexture();

    const fs::path DEFAULT_PATH {fs::current_path().parent_path()/"Resources"/"Textures"};

    Texture loadTexture(const TextureLocation& location) {
        Texture texture;

        try {
            texture = loadTexture(location.path);
        } catch (std::exception& e) {
            Logger::warning({"Texture {} not found, using default", location.path.string()});
            texture = loadDefaultTexture();
        }

        return  texture;
    }

    static Texture loadTexture(const std::filesystem::path& path) {
        Texture out;

        int channels;

        stbi_set_flip_vertically_on_load(true);

        const std::string fullPath = (DEFAULT_PATH / path).string();

        Logger::info({"Loading texture {}", fullPath});
        stbi_uc *pixels = stbi_load(
            fullPath.c_str(),
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

        ASSERT::openGlLoaded();

        glGenTextures(1, &out.id);
        glBindTexture(GL_TEXTURE_2D, out.id);

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

    static Texture loadDefaultTexture() {
        static Texture defaultTexture = loadTexture("White.png");
        return defaultTexture;
    }
}
