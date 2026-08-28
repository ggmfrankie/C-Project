//
// Created by ertls on 16.02.2026.
//
#include "EngineDefines.hpp"
#include "Test.h"
#include "Dependencies/JoltPhysics/Jolt/Jolt.h"

#include "Cpp/GameEngine.hpp"
#include "Cpp/Games/BaseGame/BaseGame.hpp"
#include "Cpp/Games/Game2D/Game2D.hpp"
#include "Cpp/Test/Test.hpp"
#include "C/Utils/DataStructures/CString.h"

#if 1
int main() {
    Test_run();
    try {
        JPH::RegisterDefaultAllocator();
        Game::BaseGame game{};
        GameEngine& gameEngine = GameEngine::New(game);
        gameEngine.init();
        gameEngine.loop();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 2;
    }

}
#endif