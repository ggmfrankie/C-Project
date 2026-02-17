//
// Created by ertls on 16.02.2026.
//

#include "Screen.h"
#include <iostream>
#include <utility>
#include <glad/gl.h>
namespace Render{
    Screen::Screen(std::string  windowName, const int width, const int height) : name(std::move(windowName)), shader("MainShader.vert", "MainShader.frag") {
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

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        windowHandle = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
        if (!windowHandle) {
            glfwTerminate();
            puts("Failed to open GLFW window");
        }

        glfwMakeContextCurrent(windowHandle);

        if (!gladLoadGL(glfwGetProcAddress)) {
            glfwDestroyWindow(windowHandle);
            glfwTerminate();
        }
        glViewport(0, 0, width, height);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glfwSetFramebufferSizeCallback(windowHandle, framebufferSizeCallback);
        shader.init();
        shader.link();
        for (auto object: objects) {
            puts("init Objects");
            object.init();
        }
    }

    void Screen::render() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.bind();
        for (auto& obj : objects) {
            obj.render();
        }
        Shader::unbind();
    }

    void Screen::addObject(Obj::Object&& object) {
        objects.emplace_back(std::move(object));
    }

    GLFWwindow* Screen::getWindowHandle() const {
        return windowHandle;
    }

    void Screen::framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
}



