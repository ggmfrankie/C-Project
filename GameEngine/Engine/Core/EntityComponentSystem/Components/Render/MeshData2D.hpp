//
// Created by Stefan on 26.10.2026.
//

#pragma once
#include <vector>

#include "Math/Vector.hpp"

namespace ggm {
    struct MeshData2D {
        std::vector<Vector2f> vertices;
        std::vector<Vector2f> uvs;
        std::vector<int> indices;
    };
}
