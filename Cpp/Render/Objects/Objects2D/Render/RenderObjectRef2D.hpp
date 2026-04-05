//
// Created by Stefan on 05.04.2026.
//

#pragma once
#include "RenderObject2D.hpp"
#include "Utils/DataStorage/SparseSet.hpp"
#include "Utils/Math/ggmdef.hpp"

namespace Obj2D {

    struct RenderObjRef2D {
    private:
        ggm::u64 id = INVALID_ID;
        ggm::SparseSet<RenderObject2D>& objects;

    public:
        RenderObjRef2D(ggm::u64 id, ggm::SparseSet<RenderObject2D>& objects) :
            id(id), objects(objects)
        {}

        [[nodiscard]] RenderObject2D& get() const {
            return objects.get(id);
        };
        [[nodiscard]] bool empty() const {
            return id == INVALID_ID;
        }
        static constexpr ggm::u64 INVALID_ID = std::numeric_limits<ggm::u64>::max();
    };
}
