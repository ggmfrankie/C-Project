//
// Created by ertls on 26.02.2026.
//

#include "BaseGame.h"

#include <vector>

#include "../Render/Objects/Object.h"
#include "../Render/Objects/Loader/ObjLoader.h"

namespace Render {
    void BaseGame::onInit() {
        auto obj = Obj::Object::getDummyObject();
        obj.moveBy(0,0, -30);
        auto obj2 = Obj::Object("grass_block\\grass_block.obj");
        obj2.moveBy(0,0, -20);

        auto obj3 = Obj::Object("ground_plane\\ground_plane.obj");

        objects->push_back(std::move(obj));
        objects->push_back(std::move(obj2));
        objects->push_back(std::move(obj3));
    }

    void BaseGame::onUpdate(float deltaTime) {
        Math::Vector3f cameraMovement{};

        if (input->isKeyDown(GLFW_KEY_W)) cameraMovement.z = -1;
        else if (input->isKeyDown(GLFW_KEY_S)) cameraMovement.z = 1;

        if (input->isKeyDown(GLFW_KEY_A)) cameraMovement.x = -1;
        else if (input->isKeyDown(GLFW_KEY_D)) cameraMovement.x = 1;

        if (input->isKeyDown(GLFW_KEY_LEFT_CONTROL)) cameraMovement.y = -1;
        else if (input->isKeyDown(GLFW_KEY_SPACE)) cameraMovement.y = 1;

        camera->moveBy(cameraMovement.x * CAMERA_POS_STEP, cameraMovement.y * CAMERA_POS_STEP, cameraMovement.z* CAMERA_POS_STEP);

        const Math::Vector2f camRot = input->getDisplaceVec();
        camera->moveRotation(camRot.x * MOUSE_SENSITIVITY, 0 , camRot.y * MOUSE_SENSITIVITY);
    }

    void BaseGame::onShutdown() {

    }

    void BaseGame::passState(EngineContext &&e) {
        camera = &e.camera;
        input = &e.input;
        objects = &e.objects;
    }
} // Render