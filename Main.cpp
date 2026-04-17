//
// Created by ertls on 16.02.2026.
//
#include "Cpp/GameEngine.hpp"
#include "Cpp/Games/BaseGame/BaseGame.hpp"

#define GAME
#ifdef GAME
int main() {
    auto game = Game::BaseGame();
    auto& gameEngine = GameEngine::New(game);
    gameEngine.init();
    gameEngine.loop();

}
#endif
