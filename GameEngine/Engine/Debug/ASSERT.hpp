//
// Created by Stefan on 24.10.2026.
//

#pragma once
#include <source_location>

namespace ggm::ASSERT {
    void openGlLoaded(const std::source_location& loc = std::source_location::current());
}
