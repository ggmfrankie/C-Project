//
// Created by Stefan on 20.10.2026.
//

#include "InputHandler.hpp"

#include <array>
#include <sys/stat.h>

#include "GuiInterface.h"
#include "DataStorage/StringView.hpp"
#include "GLFW/glfw3.h"
#include "Math/Vector.hpp"

namespace ggm::InputHandler {
    static std::array<bool, KEY_COUNT> keysPressed;
    static std::array<bool, KEY_COUNT> keysDown;
    static std::array<bool, KEY_COUNT> keysReleased;

    static int keyModifiers;

    static std::array<bool, MOUSE_BUTTON_COUNT> mouseKeysDown;
    static std::array<bool, MOUSE_BUTTON_COUNT> mouseKeysPressed;

    static bool cursorInWindow = true;

    static Vector2f lastCursorPos;
    static Vector2f cursorPos;
    static u32 lastChar;

    void init(GLFWwindow* window) {
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCharCallback(window, charCallback);
        glfwSetCursorPosCallback(window, cursorPositionCallback);
        glfwSetCursorEnterCallback(window, cursorEnterCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
    }

    void startFrame() {
        mouseKeysPressed.fill(false);
        keysPressed.fill(false);
    }

    void keyCallback(GLFWwindow* window, int key, int code, int action, int mode) {
        if (key < 0 || key >= KEY_COUNT) return;

        keyModifiers = action;

        if (action == GLFW_PRESS) {
            keysPressed[key]  = true;
            keysDown[key]     = true;
        }
        else if (action == GLFW_RELEASE) {
            keysDown[key]     = false;
            keysReleased[key] = true;
        }
        else if (action == GLFW_REPEAT) {
            keysPressed[key]  = false;
        }

        gui_keyCallback(window, key, code, action, mode);
    }

    void charCallback(GLFWwindow* window, unsigned int codepoint) {
        lastChar = codepoint;
        gui_charCallback(window, codepoint);
    }

    void cursorPositionCallback(GLFWwindow* window, double x, double y) {
        lastCursorPos = cursorPos;
        cursorPos = {static_cast<float>(x), static_cast<float>(y)};
        gui_cursorPositionCallback(window, x, y);
    }

    void cursorEnterCallback(GLFWwindow* window, int enter) {
        cursorInWindow = enter;
    }

    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        Logger::Suppress _ {};

        keyModifiers = mods;

        switch (action) {
            case GLFW_PRESS: {
                Logger::debug({"Mouse button pressed"});
                if (!mouseKeysDown[button]) {
                    mouseKeysPressed[button] = true;
                    mouseKeysDown[button] = true;
                }

            } break;
            case GLFW_RELEASE: {
                Logger::debug({"Mouse button released"});
                mouseKeysPressed[button] = false;
                mouseKeysDown[button] = false;
            } break;
            default: ;
        }
    }

    bool isKeyPressed(Key key, Modifier modifier) {
        return keysPressed[static_cast<int>(key)];
    }

    bool isKeyDown(Key key, Modifier modifier) {
        return keysDown[static_cast<int>(key)] && keyModifiers & static_cast<int>(modifier);
    }

    bool isKeyReleased(Key key, Modifier modifier) {
        return keysReleased[static_cast<int>(key)];
    }

    bool isMouseButtonPressed(MouseButton mouseButton, Modifier modifier) {
        return mouseKeysPressed[static_cast<int>(mouseButton)];
    }

    Vector2f getCursorPos() {
        return cursorPos;
    }

    Vector2f getCursorDisplacement() {
        // TODO: fix
        return cursorPos - lastCursorPos;
    }

    std::span<const bool, KEY_COUNT> getKeysPressed() {
        return keysPressed;
    }
}
