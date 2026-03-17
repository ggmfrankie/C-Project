//
// Created by ertls on 26.02.2026.
//

#include "BaseGame.hpp"

#include "GameGui.h"
#include "../../Render/Objects/Render/RenderObject.hpp"
#include "Render/Objects/Loader/OBJLoader.hpp"
#include "GuiInterface.h"
#include "Engine/CommandRegistry.hpp"
#include "Games/DefaultLayer/PhysicsHandler.hpp"
#include "Layers/RenderLayer.hpp"
#include "Render/Screen.hpp"
#include "Render/Transformation/Camera.hpp"

#include "Utils/Math/ggmdef.hpp"

template<typename T>
void toggle(T& x) {
    x = !x;
}

namespace Game {
    void BaseGame::preInit() {

        auto physics = new PhysicsHandler();
        auto render = new RenderLayer();

        auto& scene = screen->getScene();

        auto& ls = screen->getLayerStack();
        ls.pushLayer(physics);
        ls.pushLayer(render);

        std::mt19937 rng(static_cast<unsigned int>(
            std::chrono::steady_clock::now().time_since_epoch().count()
        ));

        // Position ranges
        std::uniform_real_distribution posX(-10.0f, 10.0f);
        std::uniform_real_distribution posY(0.5f, 40.0f); // avoid intersecting floor
        std::uniform_real_distribution posZ(-10.0f, 10.0f);

        // Rotation ranges (degrees)
        std::uniform_real_distribution rotPitch(0.0f, 360.0f);
        std::uniform_real_distribution rotYaw(0.0f, 360.0f);
        std::uniform_real_distribution rotRoll(0.0f, 360.0f);

        for (int i = 0; i < 500; i++) {
            // Randomized position
            ggm::Vector3f pos(posX(rng), posY(rng), posZ(rng));

            // Randomized rotation
            ggm::Vector3f rot(rotPitch(rng), rotYaw(rng), rotRoll(rng));

            scene.pushObject(
                Obj::RenderObject("grass_block\\grass_block.obj"),
                physics->newBox(1, 1, 1, pos)
            )
            .rotateToDeg(rot);
        }

        scene.pushObject(
            Obj::RenderObject("ground_plane\\ground_plane.obj"),
            physics->newBox(1000,0,1000, {0,-32, -20}, JPH::EMotionType::Static)
        );

        cRegistry->registerCommand<
            Engine::Arg<std::string>
        >(
            "say",
            [](std::string a) {
                std::cout << a << std::endl;
            }
        );

        cRegistry->registerCommand<
            Engine::Arg<float>,
            Engine::Arg<float>,
            Engine::Arg<float>
        >(
            "tp",
            [this](float a, float b, float c) {
                camera->setPos({a,b,c});
            }
        );

        cRegistry->registerCommand<
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

        cRegistry->registerCommand<
            Engine::Arg<float>
        >(
            "movespeed",
            [this](float f) {
                CAMERA_POS_STEP = f;
            }
        );
    }

    void BaseGame::postInit() {
        gui_init(screen->getWindowHandle(), screen->getWidth(), screen->getHeight(), generateGUI);
    }

    void BaseGame::onUpdate(double deltaTime) {
        ggm::Vector3f cameraMovement{};

        if (input->isKeyDown(GLFW_KEY_W)) cameraMovement.z = 1;
        else if (input->isKeyDown(GLFW_KEY_S)) cameraMovement.z = -1;

        if (input->isKeyDown(GLFW_KEY_A)) cameraMovement.x = -1;
        else if (input->isKeyDown(GLFW_KEY_D)) cameraMovement.x = 1;

        if (input->isKeyDown(GLFW_KEY_LEFT_CONTROL)) cameraMovement.y = -1;
        else if (input->isKeyDown(GLFW_KEY_SPACE)) cameraMovement.y = 1;

        if (input->wasKeyPressed(GLFW_KEY_ESCAPE)) {
            gui_toggleVisible("Home Screen");
            toggle(guiMode);
            toggleCursorMode(screen->getWindowHandle(), guiMode);
        }

        const ggm::Vector2f camRot = input->getDisplaceVec();

        if (!guiMode) {
            camera->moveBy(cameraMovement.x * CAMERA_POS_STEP, cameraMovement.y * CAMERA_POS_STEP, cameraMovement.z * CAMERA_POS_STEP);
            camera->rotateBy(camRot.x * MOUSE_SENSITIVITY, 0 , camRot.y * MOUSE_SENSITIVITY);
        }
    }

    void BaseGame::onShutdown() {

    }

    void BaseGame::passState(EngineContext &&e) {
        camera = &e.camera;
        input = &e.input;
        screen = &e.screen;
        cRegistry = &e.commandRegistry;
    }

    void BaseGame::toggleCursorMode(GLFWwindow* window, const bool guiMode) {
        if (guiMode) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
} // Render