//
// Created by Stefan on 05.04.2026.
//

#pragma once
#include "Engine/Core/PhysixBox2D/SoftBody2D.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace Obj2D {
    struct PhysicsObjectRef2D {
    private:
        ggm::u64 id = INVALID_ID;
        ggm::SparseSet<PhysixBox::SoftBody2D>& interface;

    public:
        [[nodiscard]] PhysixBox::SoftBody2D& get() const {
            return interface.get(id);
        }

        PhysicsObjectRef2D(ggm::u64 id, ggm::SparseSet<PhysixBox::SoftBody2D>& interface) :
            id(id), interface(interface)
        {}
        static constexpr ggm::u64 INVALID_ID = std::numeric_limits<ggm::u64>::max();
    };
} // Obj2D