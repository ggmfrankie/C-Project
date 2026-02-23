//
// Created by ertls on 16.02.2026.
//

#include <iostream>

#include "Cpp/GameEngine.h"

//#define GAME
#ifdef GAME
int main() {
    auto gameEngine = GameEngine();
    gameEngine.init();
    gameEngine.loop();
}
#endif
