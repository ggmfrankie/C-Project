//
// Created by ertls on 16.02.2026.
//

#pragma once
#include <stdexcept>

#include "../EngineDefines.hpp"
#include <string>
#include "GuiTypes.h"

namespace ggm {
    class IScene;
}

namespace ggm {
    class Screen {
    public:
        Screen(std::string windowName, int width, int height);

        ~Screen();

        void init();
        void render() const noexcept;
        void attachScene(IScene* scene);

        [[nodiscard]] int getWidth() const;
        [[nodiscard]] int getHeight() const;

        void update(float dt) const noexcept;
        [[nodiscard]] GLFWwindow *getWindowHandle() const;

        [[nodiscard]] IScene& getScene() const;

        class GlException: public std::runtime_error {
        public:
            explicit GlException(const std::string& msg)
                : std::runtime_error(msg) {}
        };
    private:
        static void framebufferSizeCallback(GLFWwindow *window, int width, int height) noexcept;

        GLFWwindow* mWindowHandle;
        int mWidth, mHeight;
        std::string mName;

        IScene* mScene;
    };
}
