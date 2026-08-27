//
// Created by ertls on 26.02.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "../../Utils/Math/Matrix.hpp"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

namespace Render {

    class Input {
        static constexpr int KEY_COUNT = 348;

        ggm::Vector2f m_prevMousePos{};
        ggm::Vector2f mCurrMousePos{};
        ggm::Vector2f mDisplaceVec{};

        bool mInWindow = false;

        bool mRightMouseDown = false;
        bool mLeftMouseDown = false;

        std::array<bool, KEY_COUNT> mKeysDown{};
        std::array<bool, KEY_COUNT> mKeysPressed{};
        std::array<bool, KEY_COUNT> mKeysReleased{};

        GLuint mLastChar = 0;

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
        ggm::Vector2f getMousePos() const;
        [[nodiscard]] bool isKeyDown(int key) const;

        [[nodiscard]] bool wasKeyPressed(int key) const;

        [[nodiscard]] bool wasKeyReleased(int key) const;

        [[nodiscard]] ggm::Vector2f getDisplaceVec();

        void endFrame();
    };
}