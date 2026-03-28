//
// Created by ertls on 26.02.2026.
//

#include "Input.hpp"
#include <iostream>

#include "../../../C/GuiInterface.h"

namespace Render {
    void Input::init(GLFWwindow* window) {
        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCharCallback(window, charCallback);
        glfwSetCursorPosCallback(window, cursorPositionCallback);
        glfwSetCursorEnterCallback(window, cursorEnterCallback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    ggm::Vector2f Input::getMousePos() {
        return mCurrMousePos;
    }

    void Input::keyCallback(GLFWwindow* window, const int key, const int code, const int action, const int mode) {
        if (const auto input = static_cast<Input*>(glfwGetWindowUserPointer(window))) {
            input->setLastKey(key, action, mode);
            gui_keyCallback(window, key, code, action, mode);
        }
    }

    void Input::charCallback(GLFWwindow *window, const unsigned int codepoint) {
        if (const auto input = static_cast<Input*>(glfwGetWindowUserPointer(window))) {
            input->setLastChar(codepoint);
            gui_charCallback(window, codepoint);
        }
    }

    void Input::cursorPositionCallback(GLFWwindow* window, const double x, const double y) {
        if (const auto input = static_cast<Input*>(glfwGetWindowUserPointer(window))) {
            input->setMousePos(static_cast<float>(x), static_cast<float>(y));
            gui_cursorPositionCallback(window, x, y);
        }
    }

    void Input::cursorEnterCallback(GLFWwindow* window, const int enter) {
        if (const auto input = static_cast<Input*>(glfwGetWindowUserPointer(window))) {
            input->setEntered(static_cast<bool>(enter));
        }
    }

    void Input::setMousePos(const float x, const float y) {

        float xOffset = x - mCurrMousePos.x;
        float yOffset = y - mCurrMousePos.y;

        mCurrMousePos = {x, y};

        mDisplaceVec = { yOffset, xOffset };
    }

    void Input::setLastChar(const GLuint c) {
        mLastChar = c;
    }

    void Input::setLastKey(const int key, const int mode, const int mods) {

        if (key < 0 || key >= KEY_COUNT) return;

        if (mode == GLFW_PRESS) {
            mKeysDown[key] = true;
            mKeysPressed[key] = true;
            mKeysReleased[key] = false;
        }
        else if (mode == GLFW_RELEASE) {
            mKeysDown[key] = false;
            mKeysPressed[key] = false;
            mKeysReleased[key] = true;
        }
        else if (mode == GLFW_REPEAT) {
            mKeysDown[key] = true;
            mKeysPressed[key] = false;
        }
    }

    void Input::setEntered(const bool entered) {
        mInWindow = entered;
    }


    bool Input::isKeyDown(const int key) const {
        return mKeysDown[key];
    }

    bool Input::wasKeyPressed(const int key) const {
        return mKeysPressed[key];
    }

    bool Input::wasKeyReleased(const int key) const {
        return mKeysReleased[key];
    }

    ggm::Vector2f Input::getDisplaceVec() {
        const auto vec = mDisplaceVec;
        mDisplaceVec = {};
        return vec;
    }

    void Input::endFrame() {
        for (int i = 0; i < KEY_COUNT; ++i) {
            mKeysPressed[i] = false;
            mKeysReleased[i] = false;
        }
    }

} // Render