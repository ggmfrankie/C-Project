//
// Created by ertls on 16.02.2026.
//
#include "Cpp/GameEngine.hpp"
#include "Cpp/Games/BaseGame/BaseGame.hpp"
#include "Games/Game2D/Game2D.hpp"
#include "Test/Test.hpp"

#if 1
int main() {
    //return test();
    JPH::RegisterDefaultAllocator();

    Game::BaseGame game{};
    Game::Game2D game2D{};

    GameEngine& gameEngine = GameEngine::New(game2D);
    //& gameEngine = GameEngine::New(game);
    gameEngine.init();
    gameEngine.loop();

}
#endif
 