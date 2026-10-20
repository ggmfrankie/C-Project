//
// Created by Stefan on 20.10.2026.
//

#include "PlayerController.hpp"

#include "Engine/Core/EntityComponentSystem/Systems/IO/InputHandler.hpp"
#include "GLFW/glfw3.h"

namespace ggm::PlayerController {
    void moveEntities(std::vector<Transformation>& transformations) {
        for (auto& [dimensions, position] : transformations) {
            constexpr float STEP = 5.0f;
            if (InputHandler::isKeyDown(GLFW_KEY_A)) position.x -= STEP;
            if (InputHandler::isKeyDown(GLFW_KEY_D)) position.x += STEP;

            if (InputHandler::isKeyDown(GLFW_KEY_W)) position.y -= STEP;
            if (InputHandler::isKeyDown(GLFW_KEY_S)) position.y += STEP;
        }
    }
}

