//
// Created by ertls on 17.02.2026.
//

#include "Object.h"

#include "Loader/ObjLoader.h"

namespace Obj {

    Object::Object(const std::string& objFile) {
        auto obj = OBJLoader::OBJObject(objFile);
        obj.load();
        meshes.push_back(obj.getMesh());
    }

    Object::Object(Object&& other)  noexcept: meshes(std::move(other.meshes)) {}

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