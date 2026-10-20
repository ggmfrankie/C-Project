//
// Created by ertls on 16.02.2026.
//
#include "EngineDefines.hpp"
#include "Test.hpp"
#include "Dependencies/JoltPhysics/Jolt/Jolt.h"
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/ECSTest.hpp"

#include "GameEngine/GameEngine.hpp"
#include "GameEngine/Games/BaseGame/BaseGame.hpp"
#include "GameEngine/Games/Game2D/Game2D.hpp"
#include "GameEngine/Test/Test.hpp"
#include "Utils/Test.h"
#include "_Projects_/ggmTanks/TankGame.hpp"

#if 1
int main() {
    Test_run();
    Testing::runTests();
    ggm::ECSTest::run();

    ggm::Logger::setLevel(ggm::Logger::LogLevel::DEBUG);
    try {
        JPH::RegisterDefaultAllocator();
        ggm::BaseGame game {};
        ggm::Game2D game2 {};
        ggmTanks::TankGame tankGame {};
        auto& gameEngine = ggm::GameEngine::New(tankGame);
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
