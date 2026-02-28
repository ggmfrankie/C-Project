//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_OBJECT_H
#define MIXEDPROJECT_OBJECT_H
#include <vector>

#include "../../Utils/DataStorage/ArrayList.hpp"
#include "../../Utils/Math/Quaternion.hpp"
#include "../../Utils/Math/Matrix.hpp"
#include "Geometry/Mesh.hpp"
#include "../Shader/Shader.hpp"



namespace Obj {
    class Object {
        public:
        explicit Object(const std::string &objFile);

        Object();

        Object(Object &&other) noexcept ;

        ~Object();

        void init(Render::Shader *s);

        void render() const;

        void rotateBy(float pitch, float yaw, float roll);

        void moveBy(float dx, float dy, float dz);

        void moveTo(float x, float y, float z);

        static Object getDummyObject();

        Math::Matrix4f getModelMatrix();

    private:
        Math::Quaternion rotation = Math::Quaternion::Identity();
        bool initialized = false;
        float scale = 1.0f;

        Math::Matrix4f model = Math::Matrix4f::Identity();
        bool dirty = true;

        Math::Vector3f position{0,0,0};
        Render::ArrayList<Mesh, 1> meshes{};
        Render::Shader* shader = nullptr;
    };
} // Core

#endif //MIXEDPROJECT_OBJECT_H