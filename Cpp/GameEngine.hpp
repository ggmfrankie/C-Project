//
// Created by ertls on 16.02.2026.
//
#pragma once

#include <queue>

#include "Engine/CommandRegistry.hpp"
#include "Engine/Task.hpp"

#include "Render/Screen.hpp"

namespace Game {
    class IGame;
}

class GameEngine {
public:

    ~GameEngine() = default;

    explicit GameEngine(Game::IGame& game);

    void loop();
    void init();

    void pushTask(const Engine::Task &t);

    Render::Screen &getScreen();
    Engine::CommandRegistry &getCommandRegistry();

    static GameEngine &New(Game::IGame &game);

    static GameEngine &Get();

private:
    void processTasks();

    static GameEngine* engineInstance;
    Game::IGame& game;
    Render::Screen screen;

    Engine::CommandRegistry commandRegistry;

    std::deque<Engine::Task> tasks{};
};
