//
// Created by ertls on 16.02.2026.
//

#ifndef MIXEDPROJECT_WINDOW_H
#define MIXEDPROJECT_WINDOW_H
#include <string>


#include "../Games/Scene/LayerStack.hpp"
#include "IO/Input.hpp"


namespace Render {
    class Screen {
    public:
        Screen(std::string windowName, int width, int height);

        ~Screen();

        void init();

        void render() const;

        Engine::Scene& getScene();

        Engine::LayerStack &getLayerStack() { return mLayerStack; }

        [[nodiscard]] int getWidth() const;
        [[nodiscard]] int getHeight() const;

        void update(double dt);
        [[nodiscard]] GLFWwindow *getWindowHandle() const;
        Input &getInput();

        void endFrame();

    private:
        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

        GLFWwindow* windowHandle;
        int width, height;
        std::string name;

        Input mInput;
        Engine::Scene mScene;

        Engine::LayerStack mLayerStack;
    };
}



#endif //MIXEDPROJECT_WINDOW_H