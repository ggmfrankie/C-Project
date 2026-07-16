//
// Created by ertls on 17.02.2026.
//

#include "Texture.hpp"

#include <iostream>

#include "glad/gl.h"
#include "stb/stb_image.h"

namespace Obj3D {
    namespace fs = std::filesystem;

    const std::filesystem::path Texture::defaultPath{fs::current_path().parent_path()/"Resources"/"Textures"};

    Texture::Texture(const std::filesystem::path& file):
        mPath(file)
    {}

    Texture::Texture():
        mPath(defaultPath/"ERROR.png")
    {}

    Texture::Texture(Texture&& other) noexcept:
        mTextureId(other.mTextureId),
        mPath(other.mPath)
    {}

    Texture::Texture(const Texture& other) noexcept:
        mTextureId(other.mTextureId),
        mPath(other.mPath)
    {}

    void Texture::init() {
        int width, height, channels;

        stbi_set_flip_vertically_on_load(true);

        const std::string fullPath = mPath.string();
        stbi_uc *pixels = stbi_load(fullPath.c_str(), &width, &height, &channels, 0);

        stbi_set_flip_vertically_on_load(false);

        if (!pixels) {
            std::ostringstream out;
            out << "Failed to load texture " << fullPath << "\n";
            throw std::runtime_error{out.str()};
        }

        glGenTextures(1, &mTextureId);
        glBindTexture(GL_TEXTURE_2D, mTextureId);

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
    }

    GLuint Texture::getId() const {
        return mTextureId;
    }
} // Geometry