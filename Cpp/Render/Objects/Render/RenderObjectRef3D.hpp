//
// Created by Stefan on 05.04.2026.
//

#pragma once
#include "RenderObject3D.hpp"
#include "Utils/DataStorage/SparseSet.hpp"
#include "Utils/Math/ggmdef.hpp"

namespace Obj3D {

    struct RenderObjectRef {
        static constexpr ggm::u64 INVALID_ID = std::numeric_limits<ggm::u64>::max();

        ggm::u64 id = INVALID_ID;
        ggm::SparseSet<RenderObject3D>& interface;

        [[nodiscard]] RenderObject3D& get() const {
            return interface.get(id);
        }
    };
}
