//
// Created by ertls on 16.02.2026.
//

#include "Screen.hpp"
#include <iostream>
#include <ranges>
#include <utility>
#include <algorithm>
#include <glad/gl.h>

#include "../../C/GuiInterface.h"

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

    void Screen::createShaderUniforms() {
        shader.createUniform("textureSampler");
        shader.createUniform("modelViewMatrix");
        shader.createUniform("projectionMatrix");
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
        glEnable(GL_DEPTH_TEST);
        glfwSwapInterval(1);

        glfwSetFramebufferSizeCallback(windowHandle, framebufferSizeCallback);

        shader.init();
        shader.link();

        createShaderUniforms();

        for (auto& object: objects) {
            object.getModelMatrix();
            object.init(&shader);
        }
    }

    void Screen::render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        shader.bind();

        for (auto& obj : objects) {

            Math::Matrix4f model      = obj.getModelMatrix();
            Math::Matrix4f view       = camera.getViewMatrix();
            Math::Matrix4f modelView  = view * model;

            shader.setUniform("modelViewMatrix", modelView);
            shader.setUniform("projectionMatrix",
                              Math::Matrix4f::Perspective(
                                  90.0,
                                  static_cast<float>(width) / static_cast<float>(height),
                                  0.01f,
                                  1000.0f
                                  )
                             );
            obj.render();
            //obj.rotateBy(1,1,1);
        }

        gui_update();
        gui_render();
        glfwSwapBuffers(windowHandle);
    }

    void Screen::addObject(Obj::Object&& object) {
        objects.push_back(std::move(object));
    }

    GLFWwindow* Screen::getWindowHandle() const {
        return windowHandle;
    }

    void Screen::framebufferSizeCallback(GLFWwindow* window, const int width, const int height)
    {
        glViewport(0, 0, width, height);
        gui_resizeCallback(window, width, height);
    }

    Camera& Screen::getCamera() {
        return camera;
    }

    int Screen::getWidth() const {
        return width;
    }

    int Screen::getHeight() const {
        return height;
    }

    std::vector<Obj::Object>& Screen::getObjectList() {
        return objects;
    }
}



