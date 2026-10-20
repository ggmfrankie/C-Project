//
// Created by Stefan on 11.10.2026.
//

#include "TankGame.hpp"

#include "GuiInterface.h"
#include "GLFW/glfw3.h"
#include "Render/Screen.hpp"

namespace ggmTanks {
    TankGame::TankGame() {

    }

    TankGame::~TankGame() {

    }

    void TankGame::preInit(ggm::EngineContext&& e) {
        mContext = e;

        e.screen->attachScene(&mScene);
    }

    void TankGame::postInit() {
        gui_init(mContext.screen->getWindowHandle(), mContext.screen->getWidth(), mContext.screen->getHeight(), nullptr);
        //glfwSetInputMode(mContext.screen->getWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void TankGame::onUpdate(double deltaTime) {

    }

    void TankGame::onShutdown() {

    }
} // ggmTanks