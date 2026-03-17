//
// Created by Stefan on 15.03.2026.
//

#include "GameObject.hpp"

namespace Obj {
    GameObject::GameObject(RenderObject&& ro, PhysicsObject&& po) :
        mRenderObject(std::move(ro)), mPhysicsObject(std::move(po))
    {
    }

    GameObject::GameObject(GameObject &&other) noexcept:
        mRenderObject(std::move(other.mRenderObject)),
        mPhysicsObject(std::move(other.mPhysicsObject))
    {
    }

    void GameObject::sync() {
        const auto pos = mPhysicsObject.getPosition();
        mRenderObject.moveTo(pos);

        const auto rot = mPhysicsObject.getRotation();
        mRenderObject.rotateTo(rot);
    }

    GameObject& GameObject::moveTo(const ggm::Vector3f &pos) {
        mPhysicsObject.setPosition(pos.x, pos.y, pos.z);
        return *this;
    }

    GameObject& GameObject::rotateTo(const ggm::Vector3f &rot) {
        mPhysicsObject.setRotation(rot.x, rot.y, rot.z);
        return *this;
    }

    GameObject& GameObject::rotateToDeg(const ggm::Vector3f &rot) {
        mPhysicsObject.setRotation(ggm::toRad(rot.x), ggm::toRad(rot.y), ggm::toRad(rot.z));
        return *this;
    }

}
