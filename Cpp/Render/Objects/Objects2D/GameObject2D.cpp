//
// Created by Stefan on 27.03.2026.
//

#include "GameObject2D.hpp"

#include "Render/DynamicMesh.hpp"
#include "Render/RenderObject2D.hpp"

namespace Obj2D {
    GameObject2D::GameObject2D(const RenderObjRef2D &renderObj, const PhysicsObjectRef2D &body) :
        mRenderObj(renderObj), mPhysicsObj(body)
    {}

    GameObject2D::~GameObject2D() = default;

    void GameObject2D::sync() {
        if (!mPhysicsObj || !mRenderObj) return;

        DynamicMesh& mesh = mRenderObj->get().getMesh();
        mesh.update(mPhysicsObj->get().getPositions());
    }
} // Obj