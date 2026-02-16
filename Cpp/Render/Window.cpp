//
// Created by ertls on 16.02.2026.
//

#include "Window.h"
#include <iostream>
#include <glad/gl.h>


Window::Window(const std::string& windowName, const int width, const int height): name(windowName) {
    this->width = width;
    this->height = height;
    windowHandle = nullptr;
}

Window::~Window() {
    glfwDestroyWindow(windowHandle);
    glfwTerminate();
    windowHandle = nullptr;
}

void Window::init() {
    if (!glfwInit()) {
        puts("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    windowHandle = glfwCreateWindow(width, height, this->name.c_str(), NULL, NULL);
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
}

void Window::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

GLFWwindow* Window::getWindowHandle() const {
    return windowHandle;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

