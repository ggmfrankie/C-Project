//
// Created by ertls on 16.02.2026.
//

#ifndef MIXEDPROJECT_WINDOW_H
#define MIXEDPROJECT_WINDOW_H
#include <glad/gl.h>
#include "GLFW/glfw3.h"
#include <string>

class Window {
public:
    explicit Window(const std::string& windowName, int width, int height);
    ~Window();

    void init();

    void render();

    GLFWwindow *getWindowHandle() const;

private:
    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

    GLFWwindow* windowHandle;
    int width, height;
    std::string name;
};


#endif //MIXEDPROJECT_WINDOW_H