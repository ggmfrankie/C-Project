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
        GameObject(const RenderObject &ro, const PhysicsObject &po);

        GameObject(RenderObject &&ro, PhysicsObject &&po);

        void sync();

        void moveTo(const ggm::Vector3f &pos);

        RenderObject& getRenderObject() { return mRenderObject; }
    };
} // Obj