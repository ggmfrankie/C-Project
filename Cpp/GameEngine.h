//
// Created by ertls on 16.02.2026.
//

#ifndef C_GAMEENGINE_H
#define C_GAMEENGINE_H
#include "Render/Screen.h"

class GameEngine {
public:
    GameEngine();
    ~GameEngine() = default;

    void addObject(Obj::Object &&obj);

    void loop();
    void init();

    void processInput();

private:
    Render::Screen screen;

};


#endif //C_GAMEENGINE_H