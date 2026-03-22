//
// Created by Stefan on 15.03.2026.
//

#include "GameObject.hpp"

namespace Obj {

    GameObject::GameObject(const std::pair<ggm::SparseSet<RenderObject>&, ggm::u64> &renderObj, std::optional<PhysicsObject> po) :
        mRenderObjectID(renderObj.second),
        mRenderObjects(renderObj.first),
        mPhysicsObject(std::move(po))
    {
    }

    bool GameObject::hasRenderObj() const {
        return mRenderObjectID != INVALID_ID;
    }

    RenderObject& GameObject::getRenderObj() const {
        return mRenderObjects.get(mRenderObjectID);
    }

    void GameObject::sync() {
        if (!mPhysicsObject || !hasRenderObj()) return;
        const auto pos = mPhysicsObject->getPosition();

        auto& rObj = getRenderObj();
        rObj.moveTo(pos);

        const auto rot = mPhysicsObject->getRotation();
        rObj.rotateTo(rot);
    }

    GameObject& GameObject::moveTo(const ggm::Vector3f &pos) {
        if (mPhysicsObject) mPhysicsObject->setPosition(pos.x, pos.y, pos.z);
        if (hasRenderObj()) getRenderObj().moveTo(pos);
        return *this;
    }

    GameObject& GameObject::rotateTo(const ggm::Vector3f &rot) {
        if (mPhysicsObject) mPhysicsObject->setRotation(rot.x, rot.y, rot.z);
        if (hasRenderObj()) getRenderObj().rotateTo(ggm::Quaternion::fromEuler(rot));
        return *this;
    }

    GameObject& GameObject::rotateToDeg(const ggm::Vector3f &rot) {
        rotateTo({ggm::toRad(rot.x), ggm::toRad(rot.y), ggm::toRad(rot.z)});
        return *this;
    }

}
