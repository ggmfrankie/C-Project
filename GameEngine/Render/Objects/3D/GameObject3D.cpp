//
// Created by Stefan on 15.03.2026.
//

#include "GameObject3D.hpp"

namespace ggm {
    using namespace ggm;

    GameObject3D::GameObject3D(std::optional<RenderObjectRef> renderObj, std::optional<PhysicsObjectRef3D> physicsObj) :
        mRenderObj(std::move(renderObj)),
        mPhysicsObj(std::move(physicsObj)),
        mPos(),
        mRot(Quaternion::Identity())
    {
    }

    GameObject3D::GameObject3D(RenderObjectRef renderObj, PhysicsObjectRef3D physicsObj) :
        mRenderObj(renderObj),
        mPhysicsObj(physicsObj),
        mPos(),
        mRot(Quaternion::Identity())
    {
    }

    GameObject3D::GameObject3D(RenderObjectRef renderObj) :
        mRenderObj(renderObj),
        mPhysicsObj(),
        mPos(),
        mRot(Quaternion::Identity())
    {
    }

    GameObject3D::GameObject3D(PhysicsObjectRef3D physicsObj) :
        mRenderObj(),
        mPhysicsObj(physicsObj),
        mPos(),
        mRot(Quaternion::Identity())
    {
    }

    void GameObject3D::sync() {
        if (!mPhysicsObj || !mRenderObj) return;
        auto& rObj = mRenderObj->get();

        const auto pos = mPhysicsObj->getPosition();
        mPos = pos;
        rObj.moveTo(pos);

        const auto rot = mPhysicsObj->getRotation();
        mRot = rot;
        rObj.rotateTo(rot);
    }

    GameObject3D& GameObject3D::moveTo(const Vector3f &pos) {
        if (mPhysicsObj) mPhysicsObj->setPosition(pos.x, pos.y, pos.z);
        if (mRenderObj) mRenderObj->get().moveTo(pos);
        mPos = pos;
        return *this;
    }

    GameObject3D& GameObject3D::rotateTo(const Vector3f &rot) {
        if (mPhysicsObj) mPhysicsObj->setRotation(rot.x, rot.y, rot.z);
        const auto rotation = Quaternion::fromEuler(rot);
        if (mRenderObj) mRenderObj->get().rotateTo(rotation);
        mRot = rotation;
        return *this;
    }

    GameObject3D& GameObject3D::rotateToDeg(const Vector3f &rot) {
        rotateTo({toRad(rot.x), toRad(rot.y), toRad(rot.z)});
        return *this;
    }
}
