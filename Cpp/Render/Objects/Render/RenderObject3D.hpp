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



namespace Obj3D {
    class RenderObject3D {
    public:
        explicit RenderObject3D(const std::string &objFile);
        RenderObject3D();
        RenderObject3D(RenderObject3D &&other) noexcept ;
        RenderObject3D(const RenderObject3D &other) noexcept;

        ~RenderObject3D();

        void init();
        void render() const;

        void rotateBy(float pitch, float yaw, float roll);
        void rotateTo(const ggm::Quaternion &rot);

        void moveBy(float dx, float dy, float dz);
        void moveTo(float x, float y, float z);
        void moveTo(const ggm::Vector3f &pos);

        void scaleTo(float scale);

        const ggm::Matrix4f& getModelMatrix();

        static RenderObject3D getDummyObject();

        RenderObject3D & operator=(RenderObject3D&& other) noexcept;

    protected:
        ggm::i64 mUuid{};
        ggm::Vector3f mPosition{0,0,0};
        ggm::Quaternion mRotation = ggm::Quaternion::Identity();
        bool mDirty = true;

    private:
        bool mInitialized = false;
        float mScale = 1.0f;

        ggm::Matrix4f mModel = ggm::Matrix4f::Identity();
        ggm::InlineVector<Mesh, 1> mMeshes{};
    };
} // Core

#endif //MIXEDPROJECT_OBJECT_H