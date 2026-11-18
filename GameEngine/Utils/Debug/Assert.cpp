//
// Created by Stefan on 24.10.2026.
//

#include "Assert.hpp"

#include <filesystem>
#include <iostream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

namespace ggm::Assert {
    [[noreturn]] inline void fail(const std::source_location& loc, std::string_view msg) {
        std::cerr << std::format(
                "[ASSERT FAILED]: {}:{} {}\n",
                std::filesystem::path(loc.file_name()).filename().string(),
                loc.line(),
                msg
            );
        std::exit(-1);
    }

    void openGlLoaded(const std::source_location& loc) {
        run(gladLoadGL(glfwGetProcAddress) != 0, {"OpenGL was not initialized"}, loc);
    }
}
