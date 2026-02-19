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
        Texture(Texture&& other) noexcept ;

        unsigned char *loadTexture(const std::string &file);

        void init();

        ~Texture() = default;

        private:
        std::string name;
        GLuint textureId{};

        unsigned char* data;
        int width{}, height{}, channels{};
    };
} // Geometry

#endif //MIXEDPROJECT_TEXTURE_H