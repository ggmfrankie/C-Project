//
// Created by ertls on 16.02.2026.
//

#ifndef MIXEDPROJECT_WINDOW_H
#define MIXEDPROJECT_WINDOW_H
#include <glad/gl.h>
#include "GLFW/glfw3.h"
#include <string>
#include <vector>

#include "Objects/Object.h"
#include "Objects/Geometry/Mesh.h"
#include "Shader/Shader.h"
namespace Render{
    class Screen {
    public:
        explicit Screen(std::string  windowName, int width, int height);
        ~Screen();

        void init();

        void render() const;

        void addObject(Obj::Object &&object);

        [[nodiscard]] GLFWwindow *getWindowHandle() const;

    private:
        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

        GLFWwindow* windowHandle;
        int width, height;
        std::string name;

        Shader shader;

        std::vector<Obj::Object> objects;
    };
}



#endif //MIXEDPROJECT_WINDOW_H