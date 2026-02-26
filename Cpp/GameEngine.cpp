//
// Created by ertls on 16.02.2026.
//

#include "GameEngine.h"

#include "Games/IGame.h"
#include "Render/Objects/Loader/ObjLoader.h"

GameEngine::GameEngine(Render::IGame& game) : game(game), screen("My Window", 800, 600) {
}

void GameEngine::loop() {

    while(!glfwWindowShouldClose(screen.getWindowHandle()))
    {
        glfwPollEvents();
        update();
        screen.render();
    }
    glfwTerminate();
}

void GameEngine::init() {
    game.passState({input, screen.getCamera(), screen.getObjectList()});
    game.onInit();
    screen.init();
    input.init(screen.getWindowHandle());
}

void GameEngine::update() {
    game.onUpdate(0.0);
}
