//
// Created by Stefan on 27.03.2026.
//

#include "GameObject2D.hpp"

namespace Obj2D {


    GameObject2D::GameObject2D(const RenderObj &renderObj, PhysixBox::SoftBody2D *body) :
        mRenderObj(renderObj.second, renderObj.first)
    {
        mPhysicsObj = body;
    }

    GameObject2D::~GameObject2D() = default;

    void GameObject2D::sync() {
        if (!mPhysicsObj || mRenderObj.empty()) return;

        DynamicMesh& mesh = mRenderObj.get().getMesh();
        mesh.update(mPhysicsObj->getPositions());
    }

    RenderObject2D& GameObject2D::RenderObjRef2D::get() const {
        return objects.get(ID);
    }

    bool GameObject2D::RenderObjRef2D::empty() const {
        return ID == INVALID_ID;
    }
} // Obj