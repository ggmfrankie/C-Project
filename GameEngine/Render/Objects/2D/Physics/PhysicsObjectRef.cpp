//
// Created by Stefan on 05.04.2026.
//

#include "PhysicsObjectRef.hpp"

namespace Game2D {
    PhysixBox::SoftBody2D & PhysicsObjectRef::get() const {
        return mInterface.get(mID);
    }

    PhysicsObjectRef::PhysicsObjectRef(ggm::u64 id, ggm::SparseSet<PhysixBox::SoftBody2D>& interface)  :
        mID(id),
        mInterface(interface)
    {}
} // Obj