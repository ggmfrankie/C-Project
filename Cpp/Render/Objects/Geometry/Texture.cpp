//
// Created by ertls on 17.02.2026.
//

#include "Texture.h"

#include "glad/gl.h"
#include "stb/stb_image.h"

namespace Obj {
    Texture::Texture(const std::string &file) {
        data = loadTexture(file);
        textureId = 0;
    }

    Texture::Texture() {
        data = nullptr;
        textureId = 0;
        width = 0;
        height = 0;
        channels = 0;
    }

    Texture::Texture(Texture &&other) noexcept: textureId(other.textureId), data(other.data), width(other.width), height(other.height), channels(other.channels) {
        other.channels = 0;
        other.width = 0;
        other.height = 0;
        other.textureId = 0;
    }


    unsigned char *Texture::loadTexture(const std::string &file) {

        const std::string path = "../Resources/Textures/" + file;

        unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!pixels) {
            printf("Failed to load image\n");
            exit(-3) ;
        }
        return pixels;
    }

    void Texture::init() {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexImage2D(GL_TEXTURE_2D, 0,
                     (channels == 4) ? GL_RGBA : GL_RGB,
                     width, height, 0,
                     (channels == 4) ? GL_RGBA : GL_RGB,
                     GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }

} // Geometry