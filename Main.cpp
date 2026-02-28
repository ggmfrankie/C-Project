//
// Created by ertls on 16.02.2026.
//
#include "Cpp/GameEngine.hpp"
#include "Cpp/Games/BaseGame/BaseGame.hpp"

//#define GAME
#ifdef GAME
int main() {
    auto game = Render::BaseGame();
    auto gameEngine = GameEngine(game);
    gameEngine.init();
    gameEngine.loop();

}
#endif
