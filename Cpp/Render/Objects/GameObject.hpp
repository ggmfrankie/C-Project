//
// Created by Stefan on 15.03.2026.
//

#pragma once

#include "Render/Renderer3D.hpp"
#include "Physics/PhysicsObjectRef.hpp"
#include "Render/RenderObjectRef3D.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace Obj3D {
    class GameObject {
        std::optional<RenderObjectRef> mRenderObj;
        std::optional<PhysicsObjectRef> mPhysicsObj;
        ggm::Vector3f mPos;
        ggm::Quaternion mRot;

    public:
        GameObject(std::optional<RenderObjectRef> renderObj, std::optional<PhysicsObjectRef> physicsObj);
        GameObject(RenderObjectRef renderObj, PhysicsObjectRef physicsObj);
        explicit GameObject(RenderObjectRef renderObj);
        explicit GameObject(PhysicsObjectRef physicsObj);

        void sync();

        GameObject& moveTo(const ggm::Vector3f &pos);
        GameObject& rotateTo(const ggm::Vector3f &rot);
        GameObject& rotateToDeg(const ggm::Vector3f &rot);
    };
} // Obj