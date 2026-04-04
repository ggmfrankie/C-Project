//
// Created by Stefan on 27.03.2026.
//

#pragma once

#include "Render/RenderObject2D.hpp"
#include "Engine/Core/PhysixBox2D/SoftBody2D.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace Obj2D {
    class GameObject2D {
        struct RenderObjRef2D {
            ggm::u64 ID;
            ggm::SparseSet<RenderObject2D>& objects;

            [[nodiscard]] RenderObject2D& get() const;
            bool empty() const;
        };
        RenderObjRef2D mRenderObj;
        PhysixBox::SoftBody2D* mPhysicsObj = nullptr;

    public:
        using RenderObj = std::pair<ggm::SparseSet<RenderObject2D>&, ggm::u64>;
        GameObject2D(const RenderObj &renderObj, PhysixBox::SoftBody2D* body);
        ~GameObject2D();

        void sync();
        static constexpr ggm::u64 INVALID_ID = std::numeric_limits<ggm::u64>::max();
    };
} // Obj