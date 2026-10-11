//
// Created by Stefan on 05.04.2026.
//

#include "PhysicsObjectRef2D.hpp"

namespace ggm {
    PhysixBox::SoftBody2D & PhysicsObjectRef2D::get() const {
        return mInterface.get(mID);
    }

    PhysicsObjectRef2D::PhysicsObjectRef2D(ggm::u64 id, ggm::SparseSet<PhysixBox::SoftBody2D>& interface)  :
        mID(id),
        mInterface(interface)
    {}
} // Obj