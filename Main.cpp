//
// Created by ertls on 16.02.2026.
//
#include "EngineDefines.hpp"
#include "Dependencies/JoltPhysics/Jolt/Jolt.h"

#include "GameEngine/GameEngine.hpp"
#include "GameEngine/Games/BaseGame/BaseGame.hpp"
#include "GameEngine/Games/Game2D/Game2D.hpp"
#include "GameEngine/Test/Test.hpp"
#include "GUIEngine/Utils/DataStructures/CStr.h"
#include "Utils/Test.h"

#if 1
int main() {
    Test_run();
    return 0;
    try {
        JPH::RegisterDefaultAllocator();
        Game::BaseGame game {};
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
