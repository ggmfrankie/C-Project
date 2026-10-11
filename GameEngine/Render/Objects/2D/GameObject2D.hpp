//
// Created by Stefan on 27.03.2026.
//

#pragma once
#include "EngineDefines.hpp"
#include "Physics/PhysicsObjectRef2D.hpp"
#include "Render/RenderObjectRef2D.hpp"

namespace ggm {
    class GameObject2D {
        std::optional<RenderObjRef2D> mRenderObj;
        std::optional<PhysicsObjectRef2D> mPhysicsObj;

    public:
        GameObject2D(const RenderObjRef2D &renderObj, const PhysicsObjectRef2D& body);
        ~GameObject2D();

        void update() const;
        RenderObject2D& getRenderObj() const;
        static constexpr u64 INVALID_ID = std::numeric_limits<u64>::max();
    };
}