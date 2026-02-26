//
// Created by ertls on 16.02.2026.
//

#ifndef C_GAMEENGINE_H
#define C_GAMEENGINE_H
#include "Games/IGame.h"
#include "Render/Screen.h"
#include "Render/IO/Input.h"

class GameEngine {
public:

    ~GameEngine() = default;

    explicit GameEngine(Render::IGame &game);

    void loop();
    void init();

    void update();

private:
    Render::IGame& game;
    Render::Screen screen;
    Render::Input input;
};


#endif //C_GAMEENGINE_H