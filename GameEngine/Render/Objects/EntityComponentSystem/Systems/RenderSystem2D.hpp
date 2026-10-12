//
// Created by Stefan on 12.10.2026.
//

#pragma once
#include "DataStorage/HashMap.hpp"

namespace ggm {
    class RenderSystem2D {
        struct RenderComponent {
            
        };
        HashMap<u64, RenderComponent> mComponents {};
    };
}

