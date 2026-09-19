//
// Created by ertls on 17.02.2026.
//

#pragma once

#include <filesystem>
#include <string>

#include "glad/gl.h"

namespace Obj3D {
    class Texture {
    public:
        explicit Texture(const std::filesystem::path& file);

        Texture();
        Texture(const Texture &other) noexcept;
        Texture(Texture&& other) noexcept ;

        void init();
        [[nodiscard]] GLuint getId() const;

        ~Texture() = default;

        Texture& operator=(Texture&&) noexcept = default;

    private:
        GLuint mTextureId = 0;
        std::filesystem::path mPath;

        static const std::filesystem::path defaultPath;
    };
}