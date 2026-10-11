//
// Created by ertls on 16.02.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include <vector>
#include <array>
#include "Texture3D.hpp"
#include "Utils/Math/Vector.hpp"
#include "glad/gl.h"

namespace ggm {
    class Mesh3D {
    public:
        Mesh3D(std::vector<Vector3f> &&vertices, std::vector<Vector2f> &&uvs,
             std::vector<Vector3f> &&normals,
             std::vector<GLuint> &&indices, Texture3D &&texture);

        Mesh3D(std::vector<Vector3f> &&vertices, std::vector<Vector2f> &&uvs,
             std::vector<Vector3f> &&normals,
             std::vector<GLuint> &&indices);

        Mesh3D(Mesh3D &&other) noexcept;
        Mesh3D(const Mesh3D &other) noexcept;

        ~Mesh3D();

        void init();

        void render() const;

        [[nodiscard]] bool hasTexture() const;

        static Mesh3D getDummyMesh();

    private:
        bool mInitialized = false;

        GLuint mVAO = 0;
        GLuint mEBO = 0;

        Texture3D mTexture{};

        std::array<GLuint, 3> mVBOs{};
        std::vector<Vector3f> mVertices{};
        std::vector<Vector3f> mNormals{};
        std::vector<Vector2f> mUvs{};
        std::vector<GLuint> mIndices{};
    };
}
