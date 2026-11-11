//
// Created by Stefan on 20.10.2026.
//

#include "PlayerController.hpp"

#include "Engine/Core/EntityComponentSystem/Systems/IO/InputHandler.hpp"
#include "GLFW/glfw3.h"

namespace ggm::PlayerController {
    void moveEntities(float deltaTime, std::span<PhysicsObjectCircle> physicsObjects) {
        for (auto& object : physicsObjects) {
            constexpr float STEP = 20000.f;

            const float step = STEP * deltaTime;
            if (InputHandler::isKeyDown(GLFW_KEY_A)) object.velocity.x = -step;
            if (InputHandler::isKeyDown(GLFW_KEY_D)) object.velocity.x = step;

            if (InputHandler::isKeyDown(GLFW_KEY_W)) object.velocity.y = -step;
            if (InputHandler::isKeyDown(GLFW_KEY_S)) object.velocity.y = step;
        }
    }
}

