//
// Created by Stefan on 28.03.2026.
//

#include "DynamicMesh2D.hpp"

#include <cassert>
#include <vector>

namespace ggm {
    using namespace ggm;

    DynamicMesh2D::DynamicMesh2D(const std::vector<Vector2f>& pos) :
        mVertices(pos)
    {}

    DynamicMesh2D::~DynamicMesh2D() = default;

    void DynamicMesh2D::init() {
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
        //glEnableVertexAttribArray(1);

        mNumVerts = static_cast<GLsizei>(mVertices.size());
        mVertices.clear();
        mUvs.clear();

        mTexture.init();

        mInitialized = true;
    }

    void DynamicMesh2D::render() const {
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, mTexture.id());

        glBindVertexArray(mVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, mNumVerts);
        glBindVertexArray(0);
    }

    void DynamicMesh2D::update(const std::vector<Vector2f>& newVerts) const {
        assert(newVerts.size() == mNumVerts);
        glBindBuffer(GL_ARRAY_BUFFER, mVBOs[0]);
        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            sizeof(Vector2f) * newVerts.size(),
            newVerts.data()
        );
    }

    DynamicMesh2D::VBOHandles DynamicMesh2D::getGlData() const {
        return {mVAO, mVBOs, mNumVerts};
    }
} // Obj