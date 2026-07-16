//
// Created by Stefan on 27.03.2026.
//

#include "RenderObject.hpp"

namespace Game2D {
    RenderObject::RenderObject(const ggm::Vector3f& color, const std::vector<ggm::Vector2f>& pos) :
        mMesh(pos), mColor(color)
    {}

    RenderObject::~RenderObject() = default;

    void RenderObject::init() {
        mMesh.init();
    }

    void RenderObject::render() const {
        mMesh.render();
    }

    ggm::Vector3f RenderObject::getColor() const {
        return mColor;
    }

    DynamicMesh& RenderObject::getMesh() {
        return mMesh;
    }
} // Obj