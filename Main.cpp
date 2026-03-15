//
// Created by ertls on 16.02.2026.
//
#include "Cpp/GameEngine.hpp"
#include "Cpp/Games/BaseGame/BaseGame.hpp"

#if 1
int main() {
    auto game = Game::BaseGame();
    GameEngine& gameEngine = GameEngine::New(game);
    gameEngine.init();
    gameEngine.loop();

}
#endif
