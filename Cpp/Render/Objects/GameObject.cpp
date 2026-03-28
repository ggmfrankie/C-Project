//
// Created by Stefan on 15.03.2026.
//

#include "GameObject.hpp"

namespace Obj {
    using namespace ggm;

    GameObject::GameObject(const std::pair<SparseSet<RenderObject>&, u64> &renderObj, std::optional<PhysicsObject>&& po) :
        mRenderObj(renderObj.second, renderObj.first),
        mPhysicsObject(std::move(po)),
        mPos(),
        mRot(Quaternion::Identity())
    {
    }

    bool GameObject::hasRenderObj() const {
        return mRenderObj.ID != INVALID_ID;
    }

    RenderObject& GameObject::getRenderObj() const {
        return mRenderObj.get();
    }

    void GameObject::sync() {
        if (!mPhysicsObject || !hasRenderObj()) return;
        auto& rObj = getRenderObj();

        const auto pos = mPhysicsObject->getPosition();
        mPos = pos;
        rObj.moveTo(pos);

        const auto rot = mPhysicsObject->getRotation();
        mRot = rot;
        rObj.rotateTo(rot);
    }

    GameObject& GameObject::moveTo(const Vector3f &pos) {
        if (mPhysicsObject) mPhysicsObject->setPosition(pos.x, pos.y, pos.z);
        if (hasRenderObj()) getRenderObj().moveTo(pos);
        mPos = pos;
        return *this;
    }

    GameObject& GameObject::rotateTo(const Vector3f &rot) {
        if (mPhysicsObject) mPhysicsObject->setRotation(rot.x, rot.y, rot.z);
        const auto rotation = Quaternion::fromEuler(rot);
        if (hasRenderObj()) getRenderObj().rotateTo(rotation);
        mRot = rotation;
        return *this;
    }

    GameObject& GameObject::rotateToDeg(const Vector3f &rot) {
        rotateTo({toRad(rot.x), toRad(rot.y), toRad(rot.z)});
        return *this;
    }

    RenderObject& GameObject::RenderObjRef::get() const {
        return objects.get(ID);
    }
}
