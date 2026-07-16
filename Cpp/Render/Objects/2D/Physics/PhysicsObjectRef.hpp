//
// Created by Stefan on 05.04.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "Engine/Core/PhysixBox2D/SoftBody2D.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace Game2D {
    struct PhysicsObjectRef {
    private:
        ggm::u64 mID = INVALID_ID;
        ggm::SparseSet<PhysixBox::SoftBody2D>& mInterface;

    public:
        PhysicsObjectRef(ggm::u64 id, ggm::SparseSet<PhysixBox::SoftBody2D>& interface);
        [[nodiscard]] PhysixBox::SoftBody2D& get() const;

        static constexpr ggm::u64 INVALID_ID = std::numeric_limits<ggm::u64>::max();
    };
}