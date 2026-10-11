//
// Created by Stefan on 27.03.2026.
//

#include "GameObject2D.hpp"

#include "Render/DynamicMesh2D.hpp"
#include "Render/RenderObject2D.hpp"

namespace ggm {
    GameObject2D::GameObject2D(const RenderObjRef2D &renderObj, const PhysicsObjectRef2D &body) :
        mRenderObj(renderObj),
        mPhysicsObj(body)
    {}

    GameObject2D::~GameObject2D() = default;

    void GameObject2D::update() const {
        if (!mPhysicsObj || !mRenderObj) return;

        DynamicMesh2D& mesh = mRenderObj->get().getMesh();
        mesh.update(mPhysicsObj->get().getPositions());
    }

    RenderObject2D& GameObject2D::getRenderObj() const {
        assert(mRenderObj.has_value());
        return mRenderObj->get();
    }
}
