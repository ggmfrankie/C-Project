//
// Created by Stefan on 15.03.2026.
//

#include "GameObject.hpp"

namespace Obj3D {
    using namespace ggm;

    GameObject::GameObject(std::optional<RenderObjectRef> renderObj, std::optional<PhysicsObjectRef> physicsObj) :
        mRenderObj(std::move(renderObj)),
        mPhysicsObj(std::move(physicsObj)),
        mPos(),
        mRot(Quaternion::Identity())
    {
    }

    GameObject::GameObject(RenderObjectRef renderObj, PhysicsObjectRef physicsObj) :
        mRenderObj(renderObj),
        mPhysicsObj(physicsObj),
        mPos(),
        mRot(Quaternion::Identity())
    {
    }

    GameObject::GameObject(RenderObjectRef renderObj) :
        mRenderObj(renderObj),
        mPhysicsObj(),
        mPos(),
        mRot(Quaternion::Identity())
    {
    }

    GameObject::GameObject(PhysicsObjectRef physicsObj) :
        mRenderObj(),
        mPhysicsObj(physicsObj),
        mPos(),
        mRot(Quaternion::Identity())
    {
    }

    void GameObject::sync() {
        if (!mPhysicsObj || !mRenderObj) return;
        auto& rObj = mRenderObj->get();

        const auto pos = mPhysicsObj->getPosition();
        mPos = pos;
        rObj.moveTo(pos);

        const auto rot = mPhysicsObj->getRotation();
        mRot = rot;
        rObj.rotateTo(rot);
    }

    GameObject& GameObject::moveTo(const Vector3f &pos) {
        if (mPhysicsObj) mPhysicsObj->setPosition(pos.x, pos.y, pos.z);
        if (mRenderObj) mRenderObj->get().moveTo(pos);
        mPos = pos;
        return *this;
    }

    GameObject& GameObject::rotateTo(const Vector3f &rot) {
        if (mPhysicsObj) mPhysicsObj->setRotation(rot.x, rot.y, rot.z);
        const auto rotation = Quaternion::fromEuler(rot);
        if (mRenderObj) mRenderObj->get().rotateTo(rotation);
        mRot = rotation;
        return *this;
    }

    GameObject& GameObject::rotateToDeg(const Vector3f &rot) {
        rotateTo({toRad(rot.x), toRad(rot.y), toRad(rot.z)});
        return *this;
    }
}
