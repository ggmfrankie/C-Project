//
// Created by ertls on 16.02.2026.
//
#include "EngineDefines.hpp"
#include "Dependencies/JoltPhysics/Jolt/Jolt.h"

#include "Cpp/GameEngine.hpp"
#include "Cpp/Games/BaseGame/BaseGame.hpp"
#include "Cpp/Extern/Sudoku/SudokuBoard.hpp"
#include "Cpp/Games/Game2D/Game2D.hpp"
#include "Cpp/Test/Test.hpp"

#if 1
int main() {
    JPH::RegisterDefaultAllocator();
    Game::BaseGame game{};
    Game2D::Game2D game2D{};

    GameEngine& gameEngine = GameEngine::New(game2D);
    gameEngine.init();
    gameEngine.loop();
}
#endif