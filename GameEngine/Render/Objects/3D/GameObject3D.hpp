//
// Created by Stefan on 15.03.2026.
//

#pragma once

#include "EngineDefines.hpp"
#include "Physics/PhysicsObjectRef3D.hpp"
#include "Render/RenderObjectRef3D.hpp"


namespace ggm {
    class GameObject3D {
        std::optional<RenderObjectRef> mRenderObj;
        std::optional<PhysicsObjectRef3D> mPhysicsObj;
        ggm::Vector3f mPos;
        ggm::Quaternion mRot;

    public:
        GameObject3D(std::optional<RenderObjectRef> renderObj, std::optional<PhysicsObjectRef3D> physicsObj);
        GameObject3D(RenderObjectRef renderObj, PhysicsObjectRef3D physicsObj);
        explicit GameObject3D(RenderObjectRef renderObj);
        explicit GameObject3D(PhysicsObjectRef3D physicsObj);

        void sync();

        GameObject3D& moveTo(const ggm::Vector3f &pos);
        GameObject3D& rotateTo(const ggm::Vector3f &rot);
        GameObject3D& rotateToDeg(const ggm::Vector3f &rot);
    };
} // Obj