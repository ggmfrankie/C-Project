//
// Created by ertls on 16.02.2026.
//

#include "../Render/Mesh3D.hpp"
#include "Math/Vector.hpp"

namespace ggm {
    using std::string, std::vector;
    Mesh3D::Mesh3D(std::vector<Vector3f>&& vertices,
                   std::vector<Vector2f>&& uvs,
                   std::vector<Vector3f>&& normals,
                   std::vector<GLuint>&& indices,
                   Texture3D&& texture)
            : mTexture(std::move(texture)),
              mVertices(std::move(vertices)),
              mNormals(std::move(normals)),
              mUvs(std::move(uvs)),
              mIndices(std::move(indices))
    {}

    Mesh3D::Mesh3D(std::vector<Vector3f>&& vertices,
                   std::vector<Vector2f>&& uvs,
                   std::vector<Vector3f>&& normals,
                   std::vector<GLuint>&& indices)
            : mTexture(),
              mVertices(std::move(vertices)),
              mNormals(std::move(normals)),
              mUvs(std::move(uvs)),
              mIndices(std::move(indices))
    {}


    Mesh3D::Mesh3D(Mesh3D&& other) noexcept
        : mInitialized(other.mInitialized),
          mVAO(other.mVAO),
          mEBO(other.mEBO),
          mTexture(std::move(other.mTexture)),
          mVBOs(other.mVBOs),
          mVertices(std::move(other.mVertices)),
          mNormals(std::move(other.mNormals)),
          mUvs(std::move(other.mUvs)),
          mIndices(std::move(other.mIndices)) {

        other.mInitialized = false;
        other.mVAO = 0;
        other.mEBO = 0;
        other.mVBOs = {0, 0, 0};
    }

    Mesh3D::Mesh3D(const Mesh3D &other) noexcept = default;

    Mesh3D::~Mesh3D() {
        if (mInitialized) glDeleteVertexArrays(1, &mVAO);
    }

    void Mesh3D::init() {
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);
        glGenBuffers(3, mVBOs.data());

        glBindBuffer(GL_ARRAY_BUFFER, mVBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mVertices.size() * 3, mVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, mVBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mNormals.size() * 3, mNormals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, mVBOs[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mUvs.size() * 2, mUvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glGenBuffers(1, &mEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mIndices.size(), mIndices.data(), GL_STATIC_DRAW);


        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        mVertices = {};
        mUvs = {};
        mNormals = {};

        mTexture.init();

        mInitialized = true;
    }

    void Mesh3D::render() const {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTexture.getId());

        glBindVertexArray(mVAO);
        glDrawElements(GL_TRIANGLES, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    bool Mesh3D::hasTexture() const {
        return mTexture.getId() != 0;
    }

    Mesh3D Mesh3D::getDummyMesh() {
        vector<Vector3f> vertices = {
            {0.5f, 0.5f, 0.0f},
            {0.5f, -0.5f, 0.0f},
            {-0.5f, -0.5f, 0.0f},
            {-0.5f, 0.5f, 0.0f}
        };

        vector<Vector3f> normals = {
            {0.5f, 0.5f, 0.0f},
            {0.5f, -0.5f, 0.0f},
            {-0.5f, -0.5f, 0.0f},
            {-0.5f, 0.5f, 0.0f}
        };

        vector<GLuint> indis = {
            0, 3, 1,
            1, 3, 2
        };

        vector<Vector2f> tex = {
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 0.0f},
            {0.0f, 1.0f}
        };

        Texture3D texture("../Resources/Objects/grass_block/grass_block.png");
        puts("load dummy Mesh");

        return {std::move(vertices), std::move(tex), std::move(normals), std::move(indis), std::move(texture)};
    }
}

