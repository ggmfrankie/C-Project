//
// Created by ertls on 17.02.2026.
//

#include "Object.h"

#include "Loader/ObjLoader.h"


namespace Obj {
    using Math::Matrix4f;

    Object::Object(const std::string& objFile) {
        auto obj = OBJLoader::OBJObject(objFile);
        obj.load();
        meshes.add(obj.getMesh());
    }

    Object::Object() = default;

    Object::Object(Object&& other) noexcept:
      rotation(other.rotation),
      initialized(other.initialized),
      scale(other.scale),
      model(other.model),
      dirty(other.dirty),
      position(other.position),
      meshes(std::move(other.meshes)),
      shader(other.shader)
    {}

    Object::~Object() = default;

    void Object::init(Render::Shader* s) {
        shader = s;
        initialized = true;
        dirty = true;
        getModelMatrix();
        for (auto& mesh : meshes) {
            mesh.init(shader);
        }
    }

    void Object::render() const {
        for (auto& mesh : meshes) {
            mesh.render();
        }
    }

    void Object::rotateBy(const float pitch, const float yaw, const float roll) {

        const float p = Matrix4f::toRad(pitch);
        const float y = Matrix4f::toRad(yaw);
        const float r = Matrix4f::toRad(roll);

        const auto dq = Math::Quaternion::fromEuler(p, y, r);
        rotation = (rotation * dq).normalized();

        dirty = true;
    }

    void Object::moveBy(const float dx, const float dy, const float dz) {
        position.x += dx;
        position.y += dy;
        position.z += dz;
        dirty = true;
    }

    void Object::moveTo(const float x, const float y, const float z) {
        position.x = x;
        position.y = y;
        position.z = z;
        dirty = true;
    }

    Object Object::getDummyObject() {
        Object dummy{};
        dummy.meshes << Mesh::getDummyMesh();
        return dummy;
    }

    Matrix4f Object::getModelMatrix() {
        if (dirty) {
            dirty = false;
            return model = Matrix4f::Translation(position) * Matrix4f(rotation.toMatrix()) * Matrix4f::Scale(scale);
        }
        return model;
    }


} // Core