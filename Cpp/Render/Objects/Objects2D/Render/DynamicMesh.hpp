//
// Created by Stefan on 28.03.2026.
//

#pragma once
#include "glad/gl.h"
#include "Render/Objects/Render/Texture.hpp"
#include "Utils/DataStorage/InlineVector.hpp"
#include "Utils/Math/Vector.hpp"
#include <vector>

namespace Obj2D {
    class DynamicMesh {
        GLuint mVAO = 0;
        GLuint mEBO = 0;

        bool mInitialized = false;
        GLint mNumVerts = 0;

        Obj3D::Texture mTexture;
        std::vector<ggm::Vector2f> mVertices;
        std::vector<ggm::Vector2f> mUvs;

        std::array<GLuint, 2> mVBOs{};
    public:
        explicit DynamicMesh(const std::vector<ggm::Vector2f>& pos);
        ~DynamicMesh();

        void init();
        void render() const;

        void update(const std::vector<ggm::Vector2f> &newVerts);
    };
} // Obj