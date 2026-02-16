//
// Created by ertls on 16.02.2026.
//

#ifndef C_GAMEENGINE_H
#define C_GAMEENGINE_H
#include "Render/Window.h"

class GameEngine {
public:
    GameEngine();
    ~GameEngine();
    void loop();
    void init();

    void processInput();

private:
    Window window;

};


#endif //C_GAMEENGINE_H