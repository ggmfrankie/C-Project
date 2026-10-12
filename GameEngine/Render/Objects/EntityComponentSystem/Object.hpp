//
// Created by Stefan on 11.10.2026.
//

#pragma once
#include <functional>

#include "ggmdef.hpp"

namespace ggm {
    class Object {
        u32 mId;
        u32 mGeneration;
    public:
        Object();
        ~Object();

        auto operator<=>(const Object&) const = default;

        friend struct std::hash<Object>;
    };
} // ggm

template<>
struct std::hash<ggm::Object> {
    std::size_t operator()(const ggm::Object& o) const noexcept {
        return std::hash<int>{}(o.mId);
    }
};
