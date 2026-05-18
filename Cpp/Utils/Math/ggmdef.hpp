//
// Created by ertls on 23.02.2026.
//

#pragma once
#include <numbers>

namespace ggm {
    typedef int i32;
    typedef long long i64;
    typedef unsigned char u8;
    typedef unsigned int u32;
    typedef unsigned long long u64;

    typedef float f32;
    typedef double f64;

    constexpr auto toRad(const float degree){return degree * (static_cast<float>(std::numbers::pi) / 180);};
}

