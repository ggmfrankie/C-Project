//
// Created by Stefan on 05.04.2026.
//

#include "Game2D.hpp"

#include "Utils/Utils.hpp"
#include "DebugLayer.hpp"
#include "Render/Screen.hpp"
#include "Render/Objects/Objects2D/Physics/PhysicsFactory.hpp"


namespace Game2D {
    void Game2D::preInit(Game::EngineContext &&e) {
        mCamera = &mScene.getCamera();
        mCommandRegistry = &e.commandRegistry;
        mInput = &e.input;
        mScreen = &e.screen;

        mScene.addObject({.2, .4, .6}, PhysicsFactory2D::newQuad(100, 100, {200, 100}));
        mScene.addObject({.2, .4, .6}, PhysicsFactory2D::newQuad(100, 100, {300, 100}));
        mScene.addObject({.2, .4, .6}, PhysicsFactory2D::newQuad(100, 100, {300, 200}));
        mScene.addObject({.2, 1.0, .6}, PhysicsFactory2D::newQuad(200, 400, {400, 500}, 45.0_deg));
        mScene.addObject({.2, 1.0, .3}, PhysicsFactory2D::newQuad(100, 100, {800, 500}, 50.0_deg));
        mScene.addObject({.6, 1.0, .6}, PhysicsFactory2D::newSoftBall(12, 40, {600, 300}));

        mScene.addLayer(std::unique_ptr<ILayer>(new DebugLayer()));

        mScreen->attachScene(&mScene);
    }

    void Game2D::postInit() {
        glfwSetInputMode(mScreen->getWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Game2D::onShutdown() {}

    void Game2D::onUpdate(double deltaTime) {}
}