//
// Created by Stefan on 05.04.2026.
//

#include "Game2D.hpp"

#include "Render/Screen.hpp"
#include "Render/Objects/Objects2D/Physics/PhysicsFactory2D.hpp"

namespace Game {
    void Game2D::preInit(EngineContext &&e) {
        mCamera = &mScene.getCamera();
        mCommandRegistry = &e.commandRegistry;
        mInput = &e.input;
        mScreen = &e.screen;

        mScene.addObject({.2, .4, .6}, Obj2D::PhysicsFactory2D::newQuad(100, 100, {100, 100}));
        mScene.addObject({.2, 1.0, .6}, Obj2D::PhysicsFactory2D::newQuad(200, 400, {400, 500}, ggm::toRad(45)));
        mScene.addObject({.2, 1.0, .3}, Obj2D::PhysicsFactory2D::newQuad(400, 400, {800, 500}, ggm::toRad(50)));
        //mScene.addObject({.6, 1.0, .6}, Obj2D::PhysicsFactory2D::newSoftBall(12, 40, {600, 300}));

        mScreen->attachScene(&mScene);
    }

    void Game2D::postInit() {
        glfwSetInputMode(mScreen->getWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Game2D::onShutdown() {
    }

    void Game2D::onUpdate(double deltaTime) {

    }
} // Game