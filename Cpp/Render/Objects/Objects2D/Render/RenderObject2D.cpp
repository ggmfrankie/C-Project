//
// Created by Stefan on 27.03.2026.
//

#include "RenderObject2D.hpp"

namespace Obj {
    RenderObject2D::RenderObject2D() {
    }

    RenderObject2D::~RenderObject2D() {
    }

    void RenderObject2D::init() {
        mMesh.init();
    }

    void RenderObject2D::render() {
        mMesh.render();
    }

    DynamicMesh& RenderObject2D::getMesh() {
        return mMesh;
    }
} // Obj