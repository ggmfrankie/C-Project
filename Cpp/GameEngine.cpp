//
// Created by ertls on 16.02.2026.
//

#include "GameEngine.h"

GameEngine::GameEngine(): screen("My Window", 800, 600) {}

void GameEngine::addObject(Obj::Object&& obj) {
    screen.addObject(std::move(obj));
}

void GameEngine::loop() {

    while(!glfwWindowShouldClose(screen.getWindowHandle()))
    {
        processInput();
        screen.render();
        glfwSwapBuffers(screen.getWindowHandle());
        glfwPollEvents();
    }
    glfwTerminate();
}

void GameEngine::init() {
    screen.addObject(Obj::Object());
    screen.init();
}

void GameEngine::processInput() {

}
