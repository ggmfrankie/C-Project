//
// Created by Stefan on 11.11.2026.
//

#include "GuiCallbacks.hpp"

#include "GameEngine.hpp"
#include "GuiTypes.h"

#include "GLFW/glfw3.h"


extern "C" bool ggmTanks_onClick_closeWindow(Element*) {
    glfwSetWindowShouldClose(ggm::GameEngine::Get().getScreen().getWindowHandle(), true);
    return true;
}

extern "C" bool ggmTanks_onClick_toggleDebugView(Element*) {
    static bool debug = false;
    if ((debug = !debug)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    return true;
}
