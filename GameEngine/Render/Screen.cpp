//
// Created by ertls on 16.02.2026.
//

#include "Screen.hpp"
#include <ranges>
#include <utility>
#include <algorithm>
#include <cassert>
#include <glad/gl.h>

#include "../../GUIEngine/GuiInterface.h"
#include "../Games/Scene/3D/Scene3D.hpp"
#include "Debug/Assert.hpp"
#include "Engine/Core/EntityComponentSystem/Systems/IO/InputHandler.hpp"
#include "GLFW/glfw3.h"

namespace ggm {
    Screen::Screen(std::string windowName, const int width, const int height) :
        mName(std::move(windowName)),
        mScene(nullptr)
    {
        mWidth = width;
        mHeight = height;
        mWindowHandle = nullptr;
    }

    Screen::~Screen() {
        glfwDestroyWindow(mWindowHandle);
        glfwTerminate();
    }

    void Screen::init() {
        if (!glfwInit()) {
            throw Exception{{"Failed to initialize GLFW"}};
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        mWindowHandle = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr, nullptr);
        if (!mWindowHandle) {
            glfwTerminate();
            throw Exception{{"Failed to open GLFW window"}};
        }

        glfwMakeContextCurrent(mWindowHandle);

        Assert::openGlLoaded();

        glfwSetWindowUserPointer(mWindowHandle, this);

        glViewport(0, 0, mWidth, mHeight);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glfwSwapInterval(1);

        glfwSetFramebufferSizeCallback(mWindowHandle, framebufferSizeCallback);
        glfwSetWindowRefreshCallback(mWindowHandle, gui_refreshCallback);

        InputHandler::init(mWindowHandle);

        Assert::run(mScene);
        mScene->init();
    }

    void Screen::render() const noexcept {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Assert::run(mScene != nullptr);
        mScene->render(mWidth, mHeight);
        glfwSwapBuffers(mWindowHandle);
    }

    /**
     * @brief Attaches a scene to the Screen to be rendered and processed
     * @param scene The scene that should be processed
     */
    void Screen::attachScene(IScene *scene) {
        Assert::run(scene != nullptr);
        mScene = scene;
    }

    GLFWwindow* Screen::getWindowHandle() const {
        return mWindowHandle;
    }

    IScene& Screen::getScene() const {
        return *mScene;
    }

    void Screen::framebufferSizeCallback(GLFWwindow* window, const int width, const int height) noexcept {
        const auto screen =  static_cast<Screen *>(glfwGetWindowUserPointer(window));
        screen->mWidth = width;
        screen->mHeight = height;
        glViewport(0, 0, width, height);
        gui_resizeCallback(window, width, height);
    }

    int Screen::getWidth() const {
        return mWidth;
    }

    int Screen::getHeight() const {
        return mHeight;
    }

    void Screen::update(const float dt) const noexcept {
        Assert::run(mScene != nullptr);
        mScene->update(dt);
    }
}



