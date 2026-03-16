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

    void GameObject::moveTo(const ggm::Vector3f& pos) const {
        mPhysicsObject.setPosition(pos.x, pos.y, pos.z);
    }

}
