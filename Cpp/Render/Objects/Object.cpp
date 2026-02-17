//
// Created by ertls on 17.02.2026.
//

#include "Object.h"

namespace Obj {
    Object::Object() {
        meshes.emplace_back();
    }
    Object::Object(const std::string& textureFile) {
        meshes.emplace_back(textureFile);
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