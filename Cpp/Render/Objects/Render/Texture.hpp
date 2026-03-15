//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_TEXTURE_H
#define MIXEDPROJECT_TEXTURE_H
#include <string>

#include "glad/gl.h"

namespace Obj {
    class Texture {
    public:
        explicit Texture(const std::string &file);

        Texture();

        Texture(const Texture &other);

        Texture(Texture&& other) noexcept ;

        bool hasData() const noexcept;

        unsigned char *loadTexture(const std::string &file);

        void init();
        GLuint id() const;

        ~Texture() = default;

        Texture& operator=(Texture&&) noexcept = default;

    private:
        std::string m_name;
        GLuint m_textureId = 0;

        unsigned char* m_data;
        int m_width{}, m_height{}, m_channels{};
    };
} // Geometry

#endif //MIXEDPROJECT_TEXTURE_H