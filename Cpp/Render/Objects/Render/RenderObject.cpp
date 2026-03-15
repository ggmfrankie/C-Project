//
// Created by ertls on 17.02.2026.
//

#include "RenderObject.hpp"
#include <algorithm>
#include <execution>

#include "../Loader/OBJLoader.hpp"


namespace Obj {
    using ggm::Matrix4f;

    RenderObject::RenderObject(const std::string& objFile) {
        static ggm::num::i64 id = 0;
        auto obj = OBJLoader::OBJObject(objFile);
        obj.load();
        meshes << (obj.getMesh());
        uuid = id++;
    }

    RenderObject::RenderObject() = default;

    RenderObject::RenderObject(RenderObject&& other) noexcept :
          rotation(other.rotation),
          initialized(other.initialized),
          scale(other.scale),
          model(other.model),
          dirty(other.dirty),
          position(other.position),
          meshes(std::move(other.meshes)),
          shader(other.shader) {
        uuid = other.uuid;
    }

    RenderObject::RenderObject(const RenderObject& other) noexcept :
          rotation(other.rotation),
          initialized(other.initialized),
          scale(other.scale),
          model(other.model),
          dirty(other.dirty),
          position(other.position),
          meshes(other.meshes),
          shader(other.shader) {
        uuid = other.uuid;
    }

    RenderObject::~RenderObject() = default;

    void RenderObject::init(Render::Shader* s) {
        shader = s;
        initialized = true;
        dirty = true;
        getModelMatrix();
        for (auto& mesh : meshes) {
            mesh.init(shader);
        }
    }

    void RenderObject::render() const {
        for (auto& mesh : meshes) {
            mesh.render();
        }
    }

    void RenderObject::update() {
        if (dirty) {
            model = Matrix4f::Translation(position) * Matrix4f(rotation.toMatrix()) * Matrix4f::Scale(scale);
            dirty = false;
        }
    }

    void RenderObject::rotateBy(const float pitch, const float yaw, const float roll) {

        const float p = Matrix4f::toRad(pitch);
        const float y = Matrix4f::toRad(yaw);
        const float r = Matrix4f::toRad(roll);

        const auto dq = ggm::Quaternion::fromEuler(p, y, r);
        rotation = (rotation * dq).normalized();

        dirty = true;
    }

    void RenderObject::moveBy(const float dx, const float dy, const float dz) {
        position.x += dx;
        position.y += dy;
        position.z += dz;
        dirty = true;
    }

    void RenderObject::moveTo(const float x, const float y, const float z) {
        position.x = x;
        position.y = y;
        position.z = z;
        dirty = true;
    }

    RenderObject RenderObject::getDummyObject() {
        RenderObject dummy{};
        dummy.meshes << Mesh::getDummyMesh();
        return dummy;
    }

    Matrix4f& RenderObject::getModelMatrix() {
        return model;
    }
} // Core