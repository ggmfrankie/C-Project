//
// Created by ertls on 26.02.2026.
//

#include "Input.h"
#include <iostream>

namespace Render {
    void Input::init(GLFWwindow* window) {
        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCharCallback(window, charCallback);
        glfwSetCursorPosCallback(window, cursorPositionCallback);
        glfwSetCursorEnterCallback(window, cursorEnterCallback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Input::keyCallback(GLFWwindow* window, const int key, const int code, const int action, const int mode) {
        if (const auto input = static_cast<Input*>(glfwGetWindowUserPointer(window))) {
            input->setLastKey(key, action, mode);
        }
    }

    void Input::charCallback(GLFWwindow *window, const unsigned int codepoint) {
        if (const auto input = static_cast<Input*>(glfwGetWindowUserPointer(window))) {
            input->setLastChar(codepoint);
        }
    }

    void Input::cursorPositionCallback(GLFWwindow* window, const double x, const double y) {
        if (const auto input = static_cast<Input*>(glfwGetWindowUserPointer(window))) {
            input->setMousePos(static_cast<float>(x), static_cast<float>(y));
        }
    }

    void Input::cursorEnterCallback(GLFWwindow* window, const int enter) {
        if (const auto input = static_cast<Input*>(glfwGetWindowUserPointer(window))) {
            input->setEntered(static_cast<bool>(enter));
        }
    }

    void Input::setMousePos(const float x, const float y) {

        float xOffset = m_currMousePos.x - x;
        float yOffset = m_currMousePos.y - y;

        m_currMousePos = {x, y};

        m_displaceVec = { xOffset, yOffset };
    }

    void Input::setLastChar(const GLuint c) {
        m_lastChar = c;
    }

    void Input::setLastKey(const int key, const int mode, const int mods) {

        if (key < 0 || key >= KEY_COUNT) return;

        if (mode == GLFW_PRESS) {
            keysDown[key] = true;
            keysPressed[key] = true;
        }
        else if (mode == GLFW_RELEASE) {
            keysDown[key] = false;
            keysReleased[key] = true;
        }
        else if (mode == GLFW_REPEAT) {
            keysDown[key] = true;
        }

    }

    void Input::setEntered(const bool entered) {
        m_inWindow = entered;
    }


    bool Input::isKeyDown(const int key) const {
        return keysDown[key];
    }

    bool Input::wasKeyPressed(const int key) const {
        return keysPressed[key];
    }

    bool Input::wasKeyReleased(const int key) const {
        return keysReleased[key];
    }

    Math::Vector2f Input::getDisplaceVec() const {
        return m_displaceVec;
    }

} // Render