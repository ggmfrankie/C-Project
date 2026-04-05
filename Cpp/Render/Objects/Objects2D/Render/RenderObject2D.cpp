//
// Created by Stefan on 27.03.2026.
//

#include "RenderObject2D.hpp"

namespace Obj2D {
    RenderObject2D::RenderObject2D(const ggm::Vector3f& color, const std::vector<ggm::Vector2f>& pos) :
        mMesh(pos), mColor(color)
    {}

    RenderObject2D::~RenderObject2D() = default;

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