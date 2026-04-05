//
// Created by ertls on 16.02.2026.
//

#include "Screen.hpp"
#include <ranges>
#include <utility>
#include <algorithm>
#include <cassert>
#include <glad/gl.h>

#include "../../C/GuiInterface.h"
#include "../Games/Scene/3D/Scene3D.hpp"

namespace Render {
    Screen::Screen(std::string windowName, const int width, const int height) :
        name(std::move(windowName)),
        mScene(nullptr)
    {
        this->width = width;
        this->height = height;
        windowHandle = nullptr;
    }

    Screen::~Screen() {
        glfwDestroyWindow(windowHandle);
        glfwTerminate();
    }

    void Screen::init() {
        if (!glfwInit()) {
            puts("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        windowHandle = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
        if (!windowHandle) {
            glfwTerminate();
            puts("Failed to open GLFW window");
        }

        glfwMakeContextCurrent(windowHandle);
        glfwSetWindowUserPointer(windowHandle, this);

        if (!gladLoadGL(glfwGetProcAddress)) {
            glfwDestroyWindow(windowHandle);
            glfwTerminate();
        }
        glViewport(0, 0, width, height);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glfwSwapInterval(1);

        glfwSetFramebufferSizeCallback(windowHandle, framebufferSizeCallback);

        mInput.init(windowHandle);
        mScene->init();
    }

    void Screen::render() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mScene->render(width, height);
        glfwSwapBuffers(windowHandle);
    }

    void Screen::attachScene(Game::IScene *scene) {
        mScene = scene;
    }

    GLFWwindow* Screen::getWindowHandle() const {
        return windowHandle;
    }

    Input& Screen::getInput() {
        return mInput;
    }

    void Screen::endFrame() {
        mInput.endFrame();
    }

    Game::IScene& Screen::getScene() const {
        return *mScene;
    }

    void Screen::framebufferSizeCallback(GLFWwindow* window, const int width, const int height)
    {
        const auto screen =  static_cast<Screen *>(glfwGetWindowUserPointer(window));
        screen->width = width;
        screen->height = height;
        glViewport(0, 0, width, height);
        gui_resizeCallback(window, width, height);
    }

    int Screen::getWidth() const {
        return width;
    }

    int Screen::getHeight() const {
        return height;
    }

    void Screen::update(const float dt) const {
        mScene->update(dt);
        gui_update();
    }
}




