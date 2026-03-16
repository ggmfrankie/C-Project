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


template<typename T>
void toggle(T& x) {
    x = !x;
}

namespace Game {
    void BaseGame::preInit() {

        auto physics = new PhysicsHandler();
        auto render = new RenderLayer();

        auto& ls = screen->getLayerStack();
        ls.pushLayer(physics);
        ls.pushLayer(render);

        auto obj = Obj::RenderObject::getDummyObject();
        obj.moveBy(0,0, -30);

        auto obj2 = Obj::GameObject(
            Obj::RenderObject("grass_block\\grass_block.obj"),
            physics->newBox(1,1,1, {0.5,0.5,0.5})
        );
        obj2.moveTo({0,20, -20});

        auto obj3 = Obj::GameObject(
            Obj::RenderObject("ground_plane\\ground_plane.obj"),
            physics->newBox(1,1,1, {0.5,0.5,0.5})
        );

        auto& scene = screen->getScene();
        scene.pushObject(std::move(obj2));
        //scene.pushObject(std::move(obj3));

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