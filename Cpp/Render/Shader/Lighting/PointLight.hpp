//
// Created by ertls on 11.03.2026.
//

#ifndef MIXEDPROJECT_POINTLIGHT_HPP
#define MIXEDPROJECT_POINTLIGHT_HPP
#include "Utils/Math/Vector.hpp"

namespace Render {
    class PointLight {
        ggm::Vector3f pos{};
        ggm::Vector3f color{};

    };
} // Render

#endif //MIXEDPROJECT_POINTLIGHT_HPP