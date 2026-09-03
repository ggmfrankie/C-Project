//
// Created by Stefan on 27.03.2026.
//

#pragma once
#include "EngineDefines.hpp"
#include "Physics/PhysicsObjectRef.hpp"
#include "Render/RenderObjectRef.hpp"

namespace Game2D {
    class GameObject {
        std::optional<RenderObjRef2D> mRenderObj;
        std::optional<PhysicsObjectRef> mPhysicsObj;

    public:
        GameObject(const RenderObjRef2D &renderObj, const PhysicsObjectRef& body);
        ~GameObject();

        void update() const;
        RenderObject& getRenderObj() const;
        static constexpr ggm::u64 INVALID_ID = std::numeric_limits<ggm::u64>::max();
    };
}