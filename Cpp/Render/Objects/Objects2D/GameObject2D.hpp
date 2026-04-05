//
// Created by Stefan on 27.03.2026.
//

#pragma once
#include "Physics/PhysicsObjectRef2D.hpp"
#include "Render/RenderObjectRef2D.hpp"

namespace Obj2D {
    class GameObject2D {
        std::optional<RenderObjRef2D> mRenderObj;
        std::optional<PhysicsObjectRef2D> mPhysicsObj;

    public:

        GameObject2D(const RenderObjRef2D &renderObj, const PhysicsObjectRef2D& body);
        ~GameObject2D();

        void sync();
        static constexpr ggm::u64 INVALID_ID = std::numeric_limits<ggm::u64>::max();
    };
} // Obj