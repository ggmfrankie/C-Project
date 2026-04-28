//
// Created by ertls on 16.02.2026.
//
#include <Jolt/Jolt.h>
#include "Jolt/RegisterTypes.h"
#include "Jolt/Core/Factory.h"

#include "GameEngine.hpp"
#include "GuiInterface.h"
#include "Games/IGame.hpp"
#include "Utils/Makros.h"


GameEngine* GameEngine::engineInstance = nullptr;

GameEngine::GameEngine(Game::IGame& game) :
    game(game),
    screen("My Window", 800, 600)
{}

void GameEngine::loop() {
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

        only_every_do(100, {
            gui_setText("fps display", std::to_string(1.0/currentTime).c_str());
        });

        lastFrameTime = frameStart;
        screen.endFrame();
    }
    glfwTerminate();
}

void GameEngine::init() {
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();
    game.preInit({screen.getInput(), screen, commandRegistry});

    try {
        screen.init();
    } catch (Render::Screen::GlException e) {
        std::cerr << e.what();
        return;
    }

    game.postInit();
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
