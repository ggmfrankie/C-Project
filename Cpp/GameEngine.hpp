//
// Created by ertls on 16.02.2026.
//

#ifndef C_GAMEENGINE_H
#define C_GAMEENGINE_H
#include <queue>

#include "Engine/CommandRegistry.hpp"
#include "Engine/Task.hpp"
#include "Games/IGame.hpp"
#include "Render/Screen.hpp"
#include "Render/IO/Input.hpp"

class GameEngine {
public:

    ~GameEngine() = default;

    explicit GameEngine(Render::IGame &game);

    void loop();
    void init();

    void update();

    void pushTask(const Engine::Task &t);

    void processTasks();

    Render::Screen &getScreen();
    Engine::CommandRegistry &getCommandRegistry();

    static GameEngine &New(Render::IGame &game);

    static GameEngine &Get();

private:
    static GameEngine* engineInstance;
    Render::IGame& game;
    Render::Screen screen;
    Render::Input input;
    Engine::CommandRegistry commandRegistry;

    std::deque<Engine::Task> tasks{};
};


#endif //C_GAMEENGINE_H