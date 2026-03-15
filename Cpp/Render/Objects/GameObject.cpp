//
// Created by Stefan on 15.03.2026.
//

#include "GameObject.hpp"

namespace Obj {
    GameObject::GameObject(const RenderObject& ro, const PhysicsObject& po) :
        mRenderObject(ro), mPhysicsObject(po)
    {
    }

    GameObject::GameObject(RenderObject&& ro, PhysicsObject&& po) :
        mRenderObject(std::move(ro)), mPhysicsObject(po)
    {
    }

    void GameObject::sync() {
        const auto pos = mPhysicsObject.getPosition();
        mRenderObject.moveTo(pos);

        const auto rot = mPhysicsObject.getRotation();
        mRenderObject.rotateTo(rot);
    }

    void GameObject::moveTo(const ggm::Vector3f& pos) {
        mRenderObject.moveTo(pos);
        mPhysicsObject.setPosition(pos.x, pos.y, pos.z);
    }

}
