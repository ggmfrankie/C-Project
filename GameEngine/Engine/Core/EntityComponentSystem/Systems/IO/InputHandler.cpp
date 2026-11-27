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

    StringView toString(Key e) {
        switch (e) {
            case Key::$0: return "0";
            case Key::$1: return "1";
            case Key::$2: return "2";
            case Key::$3: return "3";
            case Key::$4: return "4";
            case Key::$5: return "5";
            case Key::$6: return "6";
            case Key::$7: return "7";
            case Key::$8: return "8";
            case Key::$9: return "9";

            case Key::SPACE: return "Space";
            case Key::APOSTROPHE: return "^";
            case Key::COMMA: return ",";
            case Key::MINUS: return "-";
            case Key::PERIOD: return ".";
            case Key::SLASH: return "/";
            case Key::SEMICOLON: return ";";
            case Key::EQUAL:
                break;
            case Key::A:
                break;
            case Key::B:
                break;
            case Key::C:
                break;
            case Key::D:
                break;
            case Key::E:
                break;
            case Key::F:
                break;
            case Key::G:
                break;
            case Key::H:
                break;
            case Key::I:
                break;
            case Key::J:
                break;
            case Key::K:
                break;
            case Key::L:
                break;
            case Key::M:
                break;
            case Key::N:
                break;
            case Key::O:
                break;
            case Key::P:
                break;
            case Key::Q:
                break;
            case Key::R:
                break;
            case Key::S:
                break;
            case Key::T:
                break;
            case Key::U:
                break;
            case Key::V:
                break;
            case Key::W:
                break;
            case Key::X:
                break;
            case Key::Y:
                break;
            case Key::Z:
                break;
            case Key::ESCAPE:
                break;
            case Key::TAB:
                break;
            case Key::LEFT_CONTROL:
                break;
            case Key::LEFT:
                break;
            case Key::RIGHT:
                break;
            case Key::UP:
                break;
            case Key::DOWN:
                break;
        }
        return "Unknown";
    }

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
