//
// Created by Stefan on 20.10.2026.
//

#include "InputHandler.hpp"

#include <array>

#include "GuiInterface.h"
#include "GLFW/glfw3.h"
#include "Math/Vector.hpp"

namespace ggm::InputHandler {
    static std::array<bool, KEY_COUNT> keysPressed;
    static std::array<bool, KEY_COUNT> keysDown;
    static std::array<bool, KEY_COUNT> keysReleased;
    
    static bool cursorInWindow = true;

    static Vector2f lastCursorPos;
    static Vector2f cursorPos;
    static u32 lastChar;

    void init(GLFWwindow* window) {
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCharCallback(window, charCallback);
        glfwSetCursorPosCallback(window, cursorPositionCallback);
        glfwSetCursorEnterCallback(window, cursorEnterCallback);
    }

    void keyCallback(GLFWwindow* window, int key, int code, int action, int mode) {
        if (key < 0 || key >= KEY_COUNT) return;

        if (action == GLFW_PRESS) {
            keysPressed[key]  = true;
            keysDown[key]     = true;
            keysReleased[key] = false;
        }
        else if (action == GLFW_RELEASE) {
            keysPressed[key]  = false;
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

    bool isKeyPressed(int key) {
        return keysPressed[key];
    }

    bool isKeyDown(int key) {
        return keysDown[key];
    }

    bool isKeyReleased(int key) {
        return keysReleased[key];
    }

    Vector2f getCursorDisplacement() {
        return cursorPos - lastCursorPos;
    }

    std::array<bool, KEY_COUNT>& getKeysPressed() {
        return keysPressed;
    }
}
