//
// Created by Stefan on 11.10.2026.
//

#include "TankGame.hpp"

#include "GuiInterface.h"
#include "Render/Screen.hpp"

namespace ggmTanks {
    TankGame::TankGame() {

    }

    TankGame::~TankGame() {

    }

    void TankGame::preInit(ggm::EngineContext&& e) {
        mInput = e.input;
        mScreen = e.screen;

        mScreen->attachScene(&mScene);
    }

    void TankGame::postInit() {
        gui_init(mScreen->getWindowHandle(), mScreen->getWidth(), mScreen->getHeight(), nullptr);
        glfwSetInputMode(mScreen->getWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void TankGame::onUpdate(double deltaTime) {

    }

    void TankGame::onShutdown() {

    }
} // ggmTanks