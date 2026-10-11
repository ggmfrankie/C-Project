//
// Created by ertls on 16.02.2026.
//
#include <Jolt/Jolt.h>
#include "Jolt/RegisterTypes.h"
#include "Jolt/Core/Factory.h"

#include "GameEngine.hpp"
#include "GuiInterface.h"
#include "Games/IGame.hpp"

namespace ggm {
    GameEngine* GameEngine::mEngineInstance = nullptr;

    GameEngine::GameEngine(IGame& game) :
        mGame(game),
        mScreen("My Window", 800, 600)
    {}

    void GameEngine::loop() {
        double lastFrameTime = glfwGetTime();
        while (!glfwWindowShouldClose(mScreen.getWindowHandle())) {
            const double frameStart = glfwGetTime();
            const double delta = frameStart - lastFrameTime;

            glfwPollEvents();
            processTasks();

            mGame.onUpdate(delta);
            mScreen.update(delta);

            mScreen.render();
            mScreen.endFrame();
            lastFrameTime = frameStart;
        }
        glfwTerminate();
    }

    void GameEngine::init() {
        JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();
        mGame.preInit({.input = &mScreen.getInput(), .screen = &mScreen, .commandRegistry = &mCommandRegistry});

        try {
            mScreen.init();
        } catch (Screen::GlException e) {
            std::cerr << e.what();
            return;
        }

        mGame.postInit();
    }

    void GameEngine::pushTask(const Task& t) {
        mTasks.push_back(t);
    }

    void GameEngine::processTasks() {
        while (!mTasks.empty()) {
            auto t = mTasks.front();
            mTasks.pop_front();
            t.execute();
        }
    }

    Screen& GameEngine::getScreen() {
        return mScreen;
    }

    CommandRegistry& GameEngine::getCommandRegistry() {
        return mCommandRegistry;
    }

    GameEngine& GameEngine::New(IGame& game) {
        mEngineInstance = new GameEngine(game);
        return *mEngineInstance;
    }

    GameEngine& GameEngine::Get() {
        assert(mEngineInstance != nullptr);
        return *mEngineInstance;
    }
}
