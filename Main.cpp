//
// Created by ertls on 16.02.2026.
//
#include "Cpp/GameEngine.h"
#include "Cpp/Games/BaseGame.h"

#define GAME
#ifdef GAME
int main() {
    auto game = Render::BaseGame();
    auto gameEngine = GameEngine(game);
    gameEngine.init();
    gameEngine.loop();

}
#endif
