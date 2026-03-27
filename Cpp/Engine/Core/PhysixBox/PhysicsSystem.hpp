//
// Created by Stefan on 27.03.2026.
//

#pragma once
#include "SoftBody2D.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace PhysixBox {
    class PhysicsSystem {
        ggm::SparseSet<SoftBody2D> mBodies{16};

        void update(int steps, float dt);

    };
} // PhysicsBox