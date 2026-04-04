//
// Created by ertls on 16.02.2026.
//

#ifndef MIXEDPROJECT_WINDOW_H
#define MIXEDPROJECT_WINDOW_H
#include <string>

#include "IO/Input.hpp"


namespace Game {
    class Scene3D;
}

namespace Render {
    class Screen {
    public:
        Screen(std::string windowName, int width, int height);

        ~Screen();

        void init();
        void render() const;
        void attachScene(Game::Scene3D* scene);

        [[nodiscard]] int getWidth() const;
        [[nodiscard]] int getHeight() const;

        void update(double dt) const;
        [[nodiscard]] GLFWwindow *getWindowHandle() const;
        Input &getInput();

        void endFrame();

        Game::Scene3D& getScene() const;

    private:
        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

        GLFWwindow* windowHandle;
        int width, height;
        std::string name;

        Game::Scene3D* mScene;
        Input mInput;
    };
}



#endif //MIXEDPROJECT_WINDOW_H