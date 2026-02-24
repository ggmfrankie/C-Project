//
// Created by ertls on 17.02.2026.
//

#include "Texture.h"

#include <iostream>

#include "glad/gl.h"
#include "stb/stb_image.h"

namespace Obj {
    Texture::Texture(const std::string &file) {
        m_data = loadTexture(file);
        m_textureId = 0;
    }

    Texture::Texture() {
        m_data = nullptr;
        m_textureId = 0;
        m_width = 0;
        m_height = 0;
        m_channels = 0;
    }

    Texture::Texture(const Texture &other) {
        m_data = other.m_data;
        m_textureId = other.m_textureId;
        m_width = other.m_width;
        m_height = other.m_height;
        m_channels = other.m_channels;

    }

    Texture::Texture(Texture &&other) noexcept: m_textureId(other.m_textureId), m_data(other.m_data), m_width(other.m_width), m_height(other.m_height), m_channels(other.m_channels) {
        other.m_channels = 0;
        other.m_width = 0;
        other.m_height = 0;
        other.m_textureId = 0;
    }

    bool Texture::hasData() const noexcept {
        return m_data != nullptr;
    }

    unsigned char *Texture::loadTexture(const std::string &file) {
        stbi_set_flip_vertically_on_load(true);
        unsigned char* pixels = stbi_load(file.c_str(), &m_width, &m_height, &m_channels, 0);
        stbi_set_flip_vertically_on_load(false);
        if (!pixels) {
            std::cout << "Failed to load texture" << file << "\n";
            exit(-3) ;
        }
        return pixels;
    }

    void Texture::init() {
        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        glTexImage2D(GL_TEXTURE_2D, 0,
                     (m_channels == 4) ? GL_RGBA : GL_RGB,
                     m_width, m_height, 0,
                     (m_channels == 4) ? GL_RGBA : GL_RGB,
                     GL_UNSIGNED_BYTE, m_data);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(m_data);
        puts("init texture");
    }

    GLuint Texture::id() const {
        return m_textureId;
    }
} // Geometry