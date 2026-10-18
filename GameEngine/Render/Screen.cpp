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
            throw GlException{"Failed to initialize GLFW"};
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        mWindowHandle = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr, nullptr);
        if (!mWindowHandle) {
            glfwTerminate();
            throw GlException{"Failed to open GLFW window"};
        }

        glfwMakeContextCurrent(mWindowHandle);
        glfwSetWindowUserPointer(mWindowHandle, this);

        if (!gladLoadGL(glfwGetProcAddress)) {
            glfwDestroyWindow(mWindowHandle);
            glfwTerminate();
            throw GlException{"Failed to load OpenGL"};
        }
        glViewport(0, 0, mWidth, mHeight);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glfwSwapInterval(1);

        glfwSetFramebufferSizeCallback(mWindowHandle, framebufferSizeCallback);
        glfwSetWindowRefreshCallback(mWindowHandle, gui_refreshCallback);

        mInput.init(mWindowHandle);
        assert(mScene);
        mScene->init();
    }

    void Screen::render() const noexcept {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        assert(mScene != nullptr);
        mScene->render(mWidth, mHeight);
        glfwSwapBuffers(mWindowHandle);
    }

    //! @brief Attaches a scene to the Screen to be rendered and processed
    void Screen::attachScene(IScene *scene) {
        assert(scene != nullptr);
        mScene = scene;
    }

    GLFWwindow* Screen::getWindowHandle() const {
        return mWindowHandle;
    }

    Input& Screen::getInput() {
        return mInput;
    }

    void Screen::endFrame() {
        mInput.endFrame();
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

    void Screen::update(const float dt) const noexcept{
        assert(mScene != nullptr);
        mScene->update(dt);
    }
}



