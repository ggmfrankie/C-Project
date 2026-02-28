//
// Created by ertls on 16.02.2026.
//

#ifndef MIXEDPROJECT_WINDOW_H
#define MIXEDPROJECT_WINDOW_H
#include <glad/gl.h>
#include "GLFW/glfw3.h"
#include <string>
#include <vector>

#include "IO/Input.hpp"
#include "Objects/Object.hpp"
#include "Objects/Geometry/Mesh.hpp"
#include "Shader/Shader.hpp"
#include "Transformation/Camera.hpp"

namespace Render{
    class Screen {
    public:
        explicit Screen(std::string  windowName, int width, int height);
        ~Screen();

        void init();

        void render();

        void addObject(Obj::Object &&object);

        Camera &getCamera();

        int getWidth() const;

        int getHeight() const;

        std::vector<Obj::Object> &getObjectList();

        [[nodiscard]] GLFWwindow *getWindowHandle() const;

    private:
        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

        void createShaderUniforms();
        GLFWwindow* windowHandle;
        int width, height;
        std::string name;

        Camera camera;

        Shader shader;

        std::vector<Obj::Object> objects;
    };
}



#endif //MIXEDPROJECT_WINDOW_H