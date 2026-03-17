//
// Created by Stefan on 15.03.2026.
//

#pragma once
#include "Physics/PhysicsObject.hpp"
#include "Render/RenderObject.hpp"

namespace Obj {
    class GameObject {

        RenderObject mRenderObject;
        PhysicsObject mPhysicsObject;

    public:

        GameObject(RenderObject &&ro, PhysicsObject &&po);

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;

        GameObject(GameObject&&) noexcept;

        void sync();

        GameObject& moveTo(const ggm::Vector3f &pos);

        GameObject &rotateTo(const ggm::Vector3f &rot);

        GameObject &rotateToDeg(const ggm::Vector3f &rot);

        RenderObject& getRenderObject() { return mRenderObject; }
    };
} // Obj