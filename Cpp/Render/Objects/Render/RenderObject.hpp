//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_OBJECT_H
#define MIXEDPROJECT_OBJECT_H
#include <vector>

#include "../../../Utils/DataStorage/InlineVector.hpp"
#include "../../../Utils/Math/Quaternion.hpp"
#include "../../../Utils/Math/Matrix.hpp"
#include "Mesh.hpp"
#include "../../Shader/Shader.hpp"



namespace Obj {
    class RenderObject {
        public:
        explicit RenderObject(const std::string &objFile);

        RenderObject();

        RenderObject(RenderObject &&other) noexcept ;

        RenderObject(const RenderObject &other) noexcept;

        ~RenderObject();

        void init();

        void render() const;

        void rotateBy(float pitch, float yaw, float roll);

        void rotateTo(const ggm::Quaternion &rot);

        void moveBy(float dx, float dy, float dz);
        void moveTo(float x, float y, float z);

        void moveTo(const ggm::Vector3f &pos);

        const ggm::Matrix4f& getModelMatrix();

        static RenderObject getDummyObject();

        RenderObject & operator=(RenderObject&& other) noexcept;

    protected:
        ggm::i64 uuid{};
        ggm::Vector3f position{0,0,0};
        ggm::Quaternion rotation = ggm::Quaternion::Identity();
        bool dirty = true;

    private:
        bool initialized = false;
        float scale = 1.0f;

        ggm::Matrix4f model = ggm::Matrix4f::Identity();


        ggm::InlineVector<Mesh, 1> mMeshes{};
    };
} // Core

#endif //MIXEDPROJECT_OBJECT_H