//
// Created by Stefan on 24.10.2026.
//

#include "ASSERT.hpp"

#include <filesystem>
#include <iostream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

namespace ggm::ASSERT {
    void openGlLoaded(const std::source_location& loc) {
        if (gladLoadGL(glfwGetProcAddress) != 0) return;

        std::cerr << std::format(
                "[ASSERT FAILED]: {}:{} {}\n",
                std::filesystem::path(loc.file_name()).filename().string(),
                loc.line(),
                "OpenGL was not initialized"
            );
    }
}
