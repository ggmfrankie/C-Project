//
// Created by ertls on 26.02.2026.
//
#include <Jolt/Jolt.h>
#include "BaseGame.hpp"

#include "GameGui.h"
#include "Render/Objects/Loader/OBJLoader.hpp"
#include "GuiInterface.h"
#include "Engine/CommandRegistry.hpp"
#include "../../Render/Objects/Physics/PhysicsHandler3D.hpp"
#include "../Scene/3D/Scene3D.hpp"
#include "Render/Screen.hpp"
#include "Render/Objects/Physics/PhysicsFactory.hpp"
#include "Render/Transformation/Camera.hpp"
#include "../Controller/Player.hpp"


template<typename T>
void toggle(T& x) {
    x = !x;
}

namespace Game {
    BaseGame::BaseGame() = default;

    BaseGame::~BaseGame() = default;

    void BaseGame::preInit(EngineContext &&e) {

        mCamera = &mScene.getCamera();
        mCommandRegistry = &e.commandRegistry;
        mInput = &e.input;
        mScreen = &e.screen;

        std::mt19937 rng(static_cast<unsigned int>(
            std::chrono::steady_clock::now().time_since_epoch().count()
        ));

        // Position ranges
        std::uniform_real_distribution posX(-10.0f, 10.0f);
        std::uniform_real_distribution posY(0.5f, 20.0f);
        std::uniform_real_distribution posZ(-10.0f, 10.0f);

        // Rotation ranges (degrees)
        std::uniform_real_distribution rotPitch(0.0f, 360.0f);
        std::uniform_real_distribution rotYaw(0.0f, 360.0f);
        std::uniform_real_distribution rotRoll(0.0f, 360.0f);

        std::uniform_real_distribution size(0.5f, 5.0f);

        for (int i = 0; i < 200; i++) {
            // Randomized position
            ggm::Vector3f pos(posX(rng), posY(rng), posZ(rng));

            // Randomized rotation
            ggm::Vector3f rot(rotPitch(rng), rotYaw(rng), rotRoll(rng));

            const float scale = size(rng);
            mScene.addObject(
                "grass_block\\grass_block.obj", scale,
                Obj3D::PhysicsFactory::newBox(scale,scale,scale, pos)
            )
            .rotateToDeg(rot);
        }

        mScene.addObject(
            "ground_plane\\ground_plane.obj", 1.0f,
            Obj3D::PhysicsFactory::newBox(1000,0,1000, {0,-32, -20}, JPH::EMotionType::Static)
        );

        mScreen->attachScene(&mScene);

        //mPlayer = std::make_unique<Player>(ggm::Vector3f{0,0,0}, physics->getPhysicsSystem());
        //mPlayer->init();

        mCommandRegistry->registerCommand<
            Engine::Arg<std::string>
        >(
            "say",
            [](std::string a) {
                std::cout << a << std::endl;
            }
        );

        mCommandRegistry->registerCommand<
            Engine::Arg<float>,
            Engine::Arg<float>,
            Engine::Arg<float>
        >(
            "tp",
            [this](float a, float b, float c) {
                mCamera->setPos({a,b,c});
            }
        );

        mCommandRegistry->registerCommand<
            Engine::Arg<bool>
        >(
            "debug",
            [](bool b) {
                if (b) {
                    glDisable(GL_CULL_FACE);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                } else {
                    glEnable(GL_CULL_FACE);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
        );

        mCommandRegistry->registerCommand<
            Engine::Arg<float>
        >(
            "movespeed",
            [this](float f) {
                CAMERA_POS_STEP = f;
            }
        );
    }

    void BaseGame::postInit() {
        gui_init(mScreen->getWindowHandle(), mScreen->getWidth(), mScreen->getHeight(), generateGUI);
    }

    void BaseGame::onUpdate(double deltaTime) {
        ggm::Vector3f cameraMovement{};

        if (mInput->isKeyDown(GLFW_KEY_W)) cameraMovement.z = 1;
        else if (mInput->isKeyDown(GLFW_KEY_S)) cameraMovement.z = -1;

        if (mInput->isKeyDown(GLFW_KEY_A)) cameraMovement.x = -1;
        else if (mInput->isKeyDown(GLFW_KEY_D)) cameraMovement.x = 1;

        if (mInput->isKeyDown(GLFW_KEY_LEFT_CONTROL)) cameraMovement.y = -1;
        else if (mInput->isKeyDown(GLFW_KEY_SPACE)) cameraMovement.y = 1;

        if (mInput->wasKeyPressed(GLFW_KEY_ESCAPE)) {
            gui_toggleVisible("Home Screen");
            toggle(guiMode);
            toggleCursorMode(mScreen->getWindowHandle(), guiMode);
        }

        const ggm::Vector2f camRot = mInput->getDisplaceVec();

        if (!guiMode) {
            mCamera->moveBy(cameraMovement.x * CAMERA_POS_STEP, cameraMovement.y * CAMERA_POS_STEP, cameraMovement.z * CAMERA_POS_STEP);
            mCamera->rotateBy(camRot.x * MOUSE_SENSITIVITY, 0 , camRot.y * MOUSE_SENSITIVITY);
        }
    }

    void BaseGame::onShutdown() {

    }

    void BaseGame::toggleCursorMode(GLFWwindow* window, const bool guiMode) {
        if (guiMode) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
} // Render