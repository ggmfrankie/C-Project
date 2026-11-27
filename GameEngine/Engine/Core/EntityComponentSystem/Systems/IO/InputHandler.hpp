//
// Created by Stefan on 20.10.2026.
//

#pragma once
#include "GuiTypes.h"
#include "Keys.hpp"
#include "DataStorage/StringView.hpp"
#include "Engine/Core/EntityComponentSystem/ComponentRegistry.hpp"
#include "Engine/Core/EntityComponentSystem/Components/KeyPressEvent.hpp"
#include "Engine/Core/EntityComponentSystem/Components/KeyReleaseEvent.hpp"
#include "Math/Vector.hpp"


namespace ggm::InputHandler {
    static constexpr int KEY_COUNT = 348;
    static constexpr int MOUSE_BUTTON_COUNT = 8;

    void init(GLFWwindow* window);
    void startFrame();

    void keyCallback(GLFWwindow* window, int key, int code, int action, int mode);
    void charCallback(GLFWwindow *window, unsigned int codepoint);
    void cursorPositionCallback(GLFWwindow *window, double x, double y);
    void cursorEnterCallback(GLFWwindow *window, int enter);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    bool isKeyPressed(Key key, Modifier modifier = Modifier::ANY);
    bool isKeyDown(Key key, Modifier modifier = Modifier::ANY);
    bool isKeyReleased(Key key, Modifier modifier = Modifier::ANY);
    bool isMouseButtonPressed(MouseButton mouseButton, Modifier modifier = Modifier::ANY);

    Vector2f getCursorPos();
    Vector2f getCursorDisplacement();

    std::span<const bool, KEY_COUNT> getKeysPressed();

    template<typename... Components>
        requires ContainsType<KeyPressEvent, Components...> && ContainsType<KeyReleaseEvent, Components...>
    void update(ComponentRegistry<Components...>& registry) {
        registry.deleteEntities(All<KeyPressEvent>{});

        const auto& pressedKeys = getKeysPressed();

        for (int i = 0; i < pressedKeys.size(); ++i) {
            if (!pressedKeys[i]) continue;

            KeyPressEvent event {.key = static_cast<Key>(i)};
            Logger::debug({"KeyPressEvent: [{}]", toString(event.key)});
            registry.createEntity({}, event);
        }
    }
}
