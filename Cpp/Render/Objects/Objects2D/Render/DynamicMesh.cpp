//
// Created by Stefan on 28.03.2026.
//

#include "DynamicMesh.hpp"
#include <vector>

namespace Obj2D {
    using namespace ggm;

    DynamicMesh::DynamicMesh(const std::vector<Vector2f>& pos) :
        mVertices(pos)
    {}

    DynamicMesh::~DynamicMesh() = default;

    void DynamicMesh::init() {
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);
        glGenBuffers(3, mVBOs.data());

        glBindBuffer(GL_ARRAY_BUFFER, mVBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * mVertices.size(), mVertices.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, mVBOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * mUvs.size(), mUvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        mVertices.clear();
        mUvs.clear();

        if (mTexture.hasData()) mTexture.init();

        mInitialized = true;
    }

    void DynamicMesh::render() const {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTexture.id());

        glBindVertexArray(mVAO);
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
        glBindVertexArray(0);
    }

    void DynamicMesh::update(const std::vector<Vector2f>& newVerts) {
        glBindBuffer(GL_ARRAY_BUFFER, mVBOs[0]);
        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            sizeof(Vector2f) * newVerts.size(),
            newVerts.data()
        );
    }
} // Obj