//
// Created by Stefan on 27.03.2026.
//

#include "GameObject.hpp"

#include "Render/DynamicMesh.hpp"
#include "Render/RenderObject.hpp"

namespace Game2D {
    GameObject::GameObject(const RenderObjRef2D &renderObj, const PhysicsObjectRef &body) :
        mRenderObj(renderObj),
        mPhysicsObj(body)
    {}

    GameObject::~GameObject() = default;

    void GameObject::update() const {
        if (!mPhysicsObj || !mRenderObj) return;

        DynamicMesh& mesh = mRenderObj->get().getMesh();
        mesh.update(mPhysicsObj->get().getPositions());
    }

    RenderObject& GameObject::getRenderObj() const {
        assert(mRenderObj.has_value());
        return mRenderObj->get();
    }
}
