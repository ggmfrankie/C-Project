//
// Created by ertls on 16.02.2026.
//

#include "GameEngine.hpp"

#include "GuiInterface.h"
#include "Games/IGame.hpp"

GameEngine* GameEngine::engineInstance = nullptr;

GameEngine::GameEngine(Game::IGame& game) : game(game), screen("My Window", 800, 600) {
}

void GameEngine::loop() {

    int i = 0;
    double lastFrameTime = glfwGetTime();
    while(!glfwWindowShouldClose(screen.getWindowHandle()))
    {
        const auto frameStart = glfwGetTime();

        glfwPollEvents();
        processTasks();

        game.onUpdate(glfwGetTime() - lastFrameTime);
        screen.update(glfwGetTime() - lastFrameTime);

        screen.render();

        const auto currentTime = glfwGetTime() - frameStart;

        if (i > 100) {
            gui_setText("fps display", std::to_string(1.0/currentTime).c_str());
            i = 0;
        }
        i++;
        lastFrameTime = frameStart;
        screen.endFrame();
    }
    glfwTerminate();
}

void GameEngine::init() {
    game.passState({screen.getInput(), screen.getScene().getCamera(), screen, commandRegistry});
    game.preInit();
    screen.init();
    game.postInit();
}

void GameEngine::update() {

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

Engine::CommandRegistry& GameEngine::getCommandRegistry() {
    return commandRegistry;
}

GameEngine& GameEngine::New(Game::IGame& game) {
    engineInstance = new GameEngine(game);
    return *engineInstance;
}

GameEngine& GameEngine::Get() {
    return *engineInstance;
}
