//
// Created by ertls on 16.02.2026.
//

#ifndef MIXEDPROJECT_WINDOW_H
#define MIXEDPROJECT_WINDOW_H
#include <string>

#include "IO/Input.hpp"


namespace Game {
    class IScene;
}


namespace Render {
    class Screen {
    public:
        Screen(std::string windowName, int width, int height);

        ~Screen();

        void init();
        void render() const noexcept;
        void attachScene(Game::IScene* scene);

        [[nodiscard]] int getWidth() const;
        [[nodiscard]] int getHeight() const;

        void update(float dt) const noexcept;
        [[nodiscard]] GLFWwindow *getWindowHandle() const;
        Input &getInput();

        void endFrame();

        [[nodiscard]] Game::IScene& getScene() const;

        class GlException: public std::runtime_error {
        public:
            explicit GlException(const std::string& msg)
                : std::runtime_error(msg) {}
        };
    private:
        static void framebufferSizeCallback(GLFWwindow *window, int width, int height) noexcept;

        GLFWwindow* windowHandle;
        int width, height;
        std::string name;

        Game::IScene* mScene;
        Input mInput;
    };
}



#endif //MIXEDPROJECT_WINDOW_H