//
// Created by ertls on 17.02.2026.
//

#include "Object.h"

#include "Loader/ObjLoader.h"

namespace Obj {
    Object::Object() {
        meshes.emplace_back();
    }

    Object::Object(const std::string& objFile) {
        auto obj = Loader::OBJLoader::OBJObject(objFile);
        obj.load();
        std::cout << obj << "\n";
    }

    Object::~Object() = default;

    void Object::init() {
        initialized = true;
        for (auto& mesh : meshes) {
            mesh.init();
        }
    }

    void Object::render() const {
        for (auto& mesh : meshes) {
            mesh.render();
        }
    }



} // Core