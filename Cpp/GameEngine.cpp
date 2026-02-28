//
// Created by ertls on 16.02.2026.
//

#include "GameEngine.hpp"

#include "Games/IGame.hpp"

GameEngine::GameEngine(Render::IGame& game) : game(game), screen("My Window", 800, 600) {
}

void GameEngine::loop() {

    while(!glfwWindowShouldClose(screen.getWindowHandle()))
    {
        glfwPollEvents();
        update();
        screen.render();
        input.endFrame();
    }
    glfwTerminate();
}

void GameEngine::init() {
    game.passState({input, screen.getCamera(), screen});
    game.preInit();
    screen.init();
    input.init(screen.getWindowHandle());
    game.postInit();
}

void GameEngine::update() {
    game.onUpdate(0.0);
}
