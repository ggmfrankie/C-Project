//
// Created by Stefan on 15.03.2026.
//

#pragma once
#include "Physics/PhysicsObject.hpp"
#include "Render/RenderObject.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace Obj {
    class GameObject {
        struct RenderObjRef {
            ggm::u64 ID;
            ggm::SparseSet<RenderObject>& objects;

            [[nodiscard]] RenderObject& get() const;
        };

        RenderObjRef mRenderObj;
        std::optional<PhysicsObject> mPhysicsObject;
        ggm::Vector3f mPos;
        ggm::Quaternion mRot;

        [[nodiscard]] bool hasRenderObj() const;
        [[nodiscard]] RenderObject &getRenderObj() const;

    public:
        explicit GameObject(const std::pair<ggm::SparseSet<RenderObject>&, ggm::u64> &renderObj, std::optional<PhysicsObject>&& po);
        GameObject(const GameObject&) = delete;
        GameObject(GameObject&& other) = default;

        GameObject& operator=(const GameObject&) = delete;
        void sync();

        GameObject& moveTo(const ggm::Vector3f &pos);
        GameObject& rotateTo(const ggm::Vector3f &rot);
        GameObject& rotateToDeg(const ggm::Vector3f &rot);

        static constexpr ggm::u64 INVALID_ID = std::numeric_limits<ggm::u64>::max();

    };
} // Obj