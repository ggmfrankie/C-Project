//
// Created by ertls on 16.02.2026.
//

#include "GameEngine.h"

#include "Render/Objects/Loader/ObjLoader.h"

GameEngine::GameEngine(): screen("My Window", 800, 600) {}

void GameEngine::addObject(Obj::Object&& obj) {
    screen.addObject(std::move(obj));
}

void GameEngine::loop() {

    while(!glfwWindowShouldClose(screen.getWindowHandle()))
    {
        processInput();
        screen.render();
        glfwPollEvents();
    }
    glfwTerminate();
}

void GameEngine::init() {
    screen.addObject(Obj::Object::getDummyObject());
    screen.init();
}

void GameEngine::processInput() {

}
