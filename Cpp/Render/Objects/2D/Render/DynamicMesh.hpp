//
// Created by Stefan on 28.03.2026.
//

#pragma once
#include "EngineDefines.hpp"
#include "glad/gl.h"

#include "Utils/Math/Vector.hpp"
#include <vector>

#include "Render/Objects/3D/Render/Texture.hpp"

namespace Game2D {
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

        void update(const std::vector<ggm::Vector2f> &newVerts) const;

        struct VBOHandles {
            GLuint vao;
            const std::array<GLuint, 2>& vbos;
            GLint vertexCount;
        };
        [[nodiscard]] VBOHandles getGlData() const;
    };
} // Obj