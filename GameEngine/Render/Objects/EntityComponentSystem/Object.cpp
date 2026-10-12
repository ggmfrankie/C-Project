//
// Created by Stefan on 11.10.2026.
//

#include "Object.hpp"

namespace ggm {
    Object::Object() {
        static u64 generator = 0;
        mId = generator++;
    }

    Object::~Object() {}
} // ggm