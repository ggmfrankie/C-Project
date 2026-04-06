//
// Created by Stefan on 27.03.2026.
//

#include "Spring.hpp"

#include "Engine/Core/PhysixBox2D/PointMass.hpp"
#include "Utils/Math/Vector.hpp"

namespace PhysixBox {
    using namespace ggm;
    Spring::Spring(PointMass& first, PointMass& second, float strength, float length,  float damping) :
        first(first),
        second(second),
        strength(strength),
        length(length),
        damping(damping)
    {}
} // PhysixBox