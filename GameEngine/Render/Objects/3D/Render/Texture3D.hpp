//
// Created by ertls on 17.02.2026.
//

#pragma once

#include <filesystem>
#include <string>

#include "glad/gl.h"

namespace ggm {
    class Texture3D {
    public:
        explicit Texture3D(const std::filesystem::path& file);

        Texture3D();
        Texture3D(const Texture3D &other) noexcept;
        Texture3D(Texture3D&& other) noexcept ;

        void init();
        [[nodiscard]] GLuint getId() const;

        ~Texture3D() = default;

        Texture3D& operator=(Texture3D&&) noexcept = default;

    private:
        GLuint mTextureId = 0;
        std::filesystem::path mPath;

        static const std::filesystem::path defaultPath;
    };
}