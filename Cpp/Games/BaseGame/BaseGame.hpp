//
// Created by ertls on 26.02.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "../IGame.hpp"
#include "OtherProjects/Cpp/McpServer/Network/Server.hpp"
#include "OtherProjects/Cpp/McpServer/Mcp/McpFunctionRegistry.hpp"
#include "OtherProjects/Cpp/McpServer/Mcp/ReturnTypes.hpp"
#include "Games/Scene/3D/Scene3D.hpp"

struct GLFWwindow;


namespace Game {
    class Scene3D;
    class Player;

    class BaseGame final : public IGame{

    public:
        BaseGame();
        ~BaseGame() override;

        void preInit(EngineContext &&e) override;
        void postInit() override;

        void onUpdate(double deltaTime) override;
        void onShutdown() override;

        static nlohmann::json getBoard();

    private:
        Render::Camera* mCamera = nullptr;
        Render::Input* mInput = nullptr;
        Render::Screen* mScreen = nullptr;
        Engine::CommandRegistry* mCommandRegistry = nullptr;
        
        mcp::Server mMcpServer {"chessServer"};

        Scene3D mScene;

        bool guiMode = false;

        float CAMERA_POS_STEP = 0.03f;
        static constexpr float MOUSE_SENSITIVITY = 0.05f;

        static void toggleCursorMode(GLFWwindow *window, bool guiMode);
    };
}
