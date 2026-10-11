//
// Created by ertls on 16.02.2026.
//
#pragma once
#include "../EngineDefines.hpp"
#include <queue>

#include "Engine/CommandRegistry.hpp"
#include "Engine/Task.hpp"

#include "Render/Screen.hpp"

namespace ggm {
    class IGame;


    class GameEngine {
    public:
        ~GameEngine() = default;

        explicit GameEngine(IGame& game);

        void loop();

        void init();

        void pushTask(const Task& t);

        Screen& getScreen();

        CommandRegistry& getCommandRegistry();

        static GameEngine& New(IGame& game);

        static GameEngine& Get();

    private:
        void processTasks();

        static GameEngine* mEngineInstance;
        IGame& mGame;
        Screen mScreen;

        CommandRegistry mCommandRegistry;

        std::deque<Task> mTasks{};
    };
}
