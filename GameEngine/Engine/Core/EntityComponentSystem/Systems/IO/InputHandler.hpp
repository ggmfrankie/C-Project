//
// Created by Stefan on 20.10.2026.
//

#pragma once
#include "GuiTypes.h"
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/KeyPressEvent.hpp"
#include "Engine/Core/EntityComponentSystem/Components/KeyReleaseEvent.hpp"
#include "Math/Vector.hpp"


namespace ggm::InputHandler {
    static constexpr int KEY_COUNT = 348;

    void init(GLFWwindow* window);

    void keyCallback(GLFWwindow* window, int key, int code, int action, int mode);
    void charCallback(GLFWwindow *window, unsigned int codepoint);
    void cursorPositionCallback(GLFWwindow *window, double x, double y);
    void cursorEnterCallback(GLFWwindow *window, int enter);

    bool isKeyPressed(int key);
    bool isKeyDown(int key);
    bool isKeyReleased(int key);

    Vector2f getCursorDisplacement();

    std::array<bool, KEY_COUNT>& getKeysPressed();

    template<typename... Components>
    requires ContainsType<KeyPressEvent, Components...> && ContainsType<KeyReleaseEvent, Components...>
    [[maybe_unused]]
    void update(ComponentRegistry<Components...>& registry) {
        const auto& pressedKeys = getKeysPressed();

        for (int i = 0; i < pressedKeys.size(); ++i) {
            if (pressedKeys[i]) registry.createEntity({}, KeyPressEvent{.keycode = i});
        }
    }
}
