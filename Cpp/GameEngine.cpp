//
// Created by ertls on 16.02.2026.
//

#include "GameEngine.h"

GameEngine::GameEngine(): window("My Window", 800, 600) {

}
GameEngine::~GameEngine() {

}

void GameEngine::loop() {

    while(!glfwWindowShouldClose(window.getWindowHandle()))
    {
        processInput();
        glfwSwapBuffers(window.getWindowHandle());
        glfwPollEvents();
    }
    glfwTerminate();
}

void GameEngine::init() {
    window.init();
}

void GameEngine::processInput() {

}
