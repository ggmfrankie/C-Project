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
    auto obj = Obj::Object::getDummyObject();
    obj.moveBy(0,0, -30);
    auto obj2 = Obj::Object("grass_block/grass_block.obj");
    obj2.moveBy(0,0, -20);
    screen.addObject(std::move(obj));
    screen.addObject(std::move(obj2));
    screen.init();
}

void GameEngine::processInput() {

}
