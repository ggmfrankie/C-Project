//
// Created by ertls on 26.02.2026.
//

#include "BaseGame.hpp"

#include <vector>

#include "GameGui.h"
#include "Render/Objects/Object.hpp"
#include "Render/Objects/Loader/OBJLoader.hpp"
#include "GuiInterface.h"
#include "Render/Objects/Behaviors/TestBehavior.hpp"

template<typename T>
void toggle(T& x) {
    x = !x;
}

namespace Render {
    void BaseGame::preInit() {
        auto obj = Obj::Object::getDummyObject();
        obj.moveBy(0,0, -30);

        auto obj2 = Obj::Object("grass_block\\grass_block.obj");
        obj2.moveBy(0,0, -20);

        auto obj3 = Obj::Object("ground_plane\\ground_plane.obj");
        obj3.moveBy(0,-30, 0);

        auto obj4 = Obj::PhysicsObject::GetDummyPhysicsObject();
        obj4.rotateBy(90, 90, 90);
        obj4.moveBy(-12,0,0);

        auto obj5 = Obj::PhysicsObject::GetDummyPhysicsObject();
        obj5.attachBehavior(new Obj::TestBehavior);
        obj5.rotateBy(45, 45, 90);
        obj5.moveBy(-8,0,0);

        screen->addObject(std::move(obj));
        screen->addObject(std::move(obj2));
        screen->addObject(std::move(obj3));
        screen->addObject(std::move(obj4));
        screen->addObject(std::move(obj5));

        cRegistry->registerCommand<
            Engine::Arg<int>,
            Engine::Arg<int>
        >(
            "add",
            [](int a, int b) {
                std::cout << a + b << std::endl;
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
        gui_addTextures("GrayBox.png");
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
            camera->moveBy(cameraMovement.x * CAMERA_POS_STEP, cameraMovement.y * CAMERA_POS_STEP, cameraMovement.z* CAMERA_POS_STEP);
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