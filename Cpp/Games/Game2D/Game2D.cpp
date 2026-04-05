//
// Created by Stefan on 05.04.2026.
//

#include "Game2D.hpp"

#include "Render/Objects/Objects2D/Physics/PhysicsFactory2D.hpp"

namespace Game {
    void Game2D::preInit(EngineContext &&e) {
        mCamera = &mScene.getCamera();
        mCommandRegistry = &e.commandRegistry;
        mInput = &e.input;
        mScreen = &e.screen;

        mScene.addObject({.2, .4, .6}, Obj2D::PhysicsFactory2D::newQuad(10, 10, {10, 10}));
    }

    void Game2D::postInit() {
    }

    void Game2D::onShutdown() {
    }

    void Game2D::onUpdate(double deltaTime) {
    }
} // Game