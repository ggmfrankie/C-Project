//
// Created by ertls on 16.02.2026.
//

#include "GameEngine.hpp"

#include "GuiInterface.h"
#include "Games/IGame.hpp"

GameEngine* GameEngine::engineInstance = nullptr;

GameEngine::GameEngine(Render::IGame& game) : game(game), screen("My Window", 800, 600) {
}

void GameEngine::loop() {

    int i = 0;
    while(!glfwWindowShouldClose(screen.getWindowHandle()))
    {
        auto lastTime = glfwGetTime();
        glfwPollEvents();
        processTasks();
        update();
        screen.render();
        input.endFrame();
        auto currentTime = glfwGetTime() - lastTime;

        if (i > 100) {
            gui_setText("fps display", std::to_string(1.0/currentTime).c_str());
            i = 0;
        }
        i++;
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

void GameEngine::pushTask(const Engine::Task& t) {
    tasks.push_back(t);
}

void GameEngine::processTasks() {
    while (!tasks.empty()) {
        auto t = tasks.front();
        tasks.pop_front();
        t.execute();
    }
}

Render::Screen& GameEngine::getScreen() {
    return screen;
}

GameEngine& GameEngine::New(Render::IGame& game) {
    engineInstance = new GameEngine(game);
    return *engineInstance;
}

GameEngine& GameEngine::Get() {
    return *engineInstance;
}
