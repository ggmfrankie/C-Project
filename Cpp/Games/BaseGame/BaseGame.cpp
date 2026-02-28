//
// Created by ertls on 26.02.2026.
//

#include "BaseGame.hpp"

#include <vector>

#include "GameGui.h"
#include "Render/Objects/Object.hpp"
#include "Render/Objects/Loader/OBJLoader.hpp"
#include "GuiInterface.h"
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

        screen->addObject(std::move(obj));
        screen->addObject(std::move(obj2));
        screen->addObject(std::move(obj3));

    }

    void BaseGame::postInit() {
        gui_init(screen->getWindowHandle(), screen->getWidth(), screen->getHeight(), generateGUI);
    }

    void BaseGame::onUpdate(float deltaTime) {
        Math::Vector3f cameraMovement{};

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

        const Math::Vector2f camRot = input->getDisplaceVec();

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
    }

    void BaseGame::toggleCursorMode(GLFWwindow* window, const bool guiMode) {
        if (guiMode) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
} // Render