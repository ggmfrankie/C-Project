//
// Created by ertls on 16.02.2026.
//

#ifndef MIXEDPROJECT_WINDOW_H
#define MIXEDPROJECT_WINDOW_H
#include <string>
#include <vector>

#include "IO/Input.hpp"
#include "Objects/Object.hpp"
#include "Objects/Physics/PhysicsObject.hpp"
#include "Shader/Shader.hpp"
#include "Transformation/Camera.hpp"

namespace Render {
    class Screen {
    public:
        Screen(std::string windowName, int width, int height);

        ~Screen();

        void init();

        void render();

        void addObject(Obj::Object &&object);
        void addObject(Obj::PhysicsObject &&object);

        Camera &getCamera();

        int getWidth() const;
        int getHeight() const;

        std::vector<Obj::Object> &getObjectList();

        void update(double dt);
        [[nodiscard]] GLFWwindow *getWindowHandle() const;
        Input &getInput();

        void endFrame();

    private:
        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

        void createShaderUniforms();
        GLFWwindow* windowHandle;
        int width, height;
        std::string name;

        Camera camera;
        Input input;
        Shader shader;

        std::vector<Obj::Object> objects;
        std::vector<Obj::PhysicsObject> pObjects;
    };
}



#endif //MIXEDPROJECT_WINDOW_H