//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_OBJECT_H
#define MIXEDPROJECT_OBJECT_H
#include <vector>

#include "../../Utils/DataStorage/InlineVector.hpp"
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

        Object(const Object &other) noexcept;

        virtual ~Object();

        virtual void init(Render::Shader *s);

        void render() const;

        virtual void update();

        virtual void rotateBy(float pitch, float yaw, float roll);
        virtual void moveBy(float dx, float dy, float dz);
        virtual void moveTo(float x, float y, float z);

        static Object getDummyObject();
        ggm::Matrix4f& getModelMatrix();

    protected:
        ggm::num::i64 uuid{};
        ggm::Vector3f position{0,0,0};
        ggm::Quaternion rotation = ggm::Quaternion::Identity();
        bool dirty = true;

    private:
        bool initialized = false;
        float scale = 1.0f;

        ggm::Matrix4f model = ggm::Matrix4f::Identity();


        ggm::InlineVector<Mesh, 1> meshes{};
        Render::Shader* shader = nullptr;
    };
} // Core

#endif //MIXEDPROJECT_OBJECT_H