//
// Created by Stefan on 05.04.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "RenderObject2D.hpp"
#include "Utils/DataStorage/SparseSet.hpp"
#include "Utils/ggmdef.hpp"

namespace ggm {

    struct RenderObjRef2D {
    private:
        u64 id = INVALID_ID;
        SparseSet<RenderObject2D>& objects;

    public:
        RenderObjRef2D(u64 id, SparseSet<RenderObject2D>& objects) :
            id(id), objects(objects)
        {}

        [[nodiscard]] RenderObject2D& get() const {
            return objects.get(id);
        };
        [[nodiscard]] bool empty() const {
            return id == INVALID_ID;
        }
        static constexpr u64 INVALID_ID = std::numeric_limits<u64>::max();
    };
}
