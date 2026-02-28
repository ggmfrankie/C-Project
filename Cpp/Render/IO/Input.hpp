//
// Created by ertls on 26.02.2026.
//

#ifndef MIXEDPROJECT_MOUSEINPUT_H
#define MIXEDPROJECT_MOUSEINPUT_H
#include "../../Utils/Math/Matrix.hpp"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

namespace Render {

    class Input {
        static constexpr int KEY_COUNT = 348;

        Math::Vector2f m_prevMousePos{};
        Math::Vector2f m_currMousePos{};
        Math::Vector2f m_displaceVec{};

        bool m_inWindow = false;

        bool m_rightMouseDown = false;
        bool m_leftMouseDown = false;

        bool keysDown[KEY_COUNT]{};
        bool keysPressed[KEY_COUNT]{};
        bool keysReleased[KEY_COUNT]{};

        GLuint m_lastChar = 0;

        static void keyCallback(GLFWwindow* window, int key, int code, int action, int mode);

        static void charCallback(GLFWwindow *window, unsigned int codepoint);

        static void cursorPositionCallback(GLFWwindow *window, double x, double y);

        static void cursorEnterCallback(GLFWwindow *window, int enter);

        void setMousePos(float x, float y);

        void setLastChar(GLuint c);

        void setLastKey(int key, int mode, int mods);

        void setEntered(bool entered);



    public:
        void init(GLFWwindow *window);
        Math::Vector2f getMousePos();
        [[nodiscard]] bool isKeyDown(int key) const;

        [[nodiscard]] bool wasKeyPressed(int key) const;

        [[nodiscard]] bool wasKeyReleased(int key) const;

        [[nodiscard]] Math::Vector2f getDisplaceVec();

        void endFrame();
    };
} // Render

#endif //MIXEDPROJECT_MOUSEINPUT_H