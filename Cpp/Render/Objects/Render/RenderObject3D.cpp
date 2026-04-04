//
// Created by ertls on 17.02.2026.
//

#include "RenderObject3D.hpp"
#include <algorithm>
#include <execution>

#include "../Loader/OBJLoader.hpp"


namespace Obj3D {
    using ggm::Matrix4f;

    RenderObject3D::RenderObject3D(const std::string& objFile) {
        static ggm::i64 id = 0;
        auto obj = OBJLoader::OBJObject(objFile);
        obj.load();
        mMeshes << (obj.getMesh());
        mUuid = id++;
    }

    RenderObject3D::RenderObject3D() = default;

    RenderObject3D::RenderObject3D(RenderObject3D&& other) noexcept :
          mPosition(other.mPosition),
          mRotation(other.mRotation),
          mDirty(other.mDirty),
          mInitialized(other.mInitialized),
          mScale(other.mScale),
          mModel(other.mModel),
          mMeshes(std::move(other.mMeshes))
    {
        mUuid = other.mUuid;
    }

    RenderObject3D::RenderObject3D(const RenderObject3D& other) noexcept :
          mPosition(other.mPosition),
          mRotation(other.mRotation),
          mDirty(other.mDirty),
          mInitialized(other.mInitialized),
          mScale(other.mScale),
          mModel(other.mModel),
          mMeshes(other.mMeshes)
    {
        mUuid = other.mUuid;
    }

    RenderObject3D::~RenderObject3D() = default;

    void RenderObject3D::init() {
        mInitialized = true;
        mDirty = true;
        getModelMatrix();
        for (auto& mesh : mMeshes) {
            mesh.init();
        }
    }

    void RenderObject3D::render() const {
        for (auto& mesh : mMeshes) {
            mesh.render();
        }
    }

    void RenderObject3D::rotateBy(const float pitch, const float yaw, const float roll) {

        const float p = ggm::toRad(pitch);
        const float y = ggm::toRad(yaw);
        const float r = ggm::toRad(roll);

        const auto dq = ggm::Quaternion::fromEuler(p, y, r);
        mRotation = (mRotation * dq).normalized();

        mDirty = true;
    }

    void RenderObject3D::rotateTo(const ggm::Quaternion& rot) {
        mRotation = rot;
        mDirty = true;
    }

    void RenderObject3D::moveBy(const float dx, const float dy, const float dz) {
        mPosition.x += dx;
        mPosition.y += dy;
        mPosition.z += dz;
        mDirty = true;
    }

    void RenderObject3D::moveTo(const float x, const float y, const float z) {
        mPosition.x = x;
        mPosition.y = y;
        mPosition.z = z;
        mDirty = true;
    }

    void RenderObject3D::moveTo(const ggm::Vector3f& pos) {
        mPosition = pos;
        mDirty = true;
    }

    void RenderObject3D::scaleTo(const float scale) {
        mScale = scale;
    }

    RenderObject3D RenderObject3D::getDummyObject() {
        RenderObject3D dummy{};
        dummy.mMeshes << Mesh::getDummyMesh();
        return dummy;
    }

    RenderObject3D& RenderObject3D::operator=(RenderObject3D&& other) noexcept {
        mMeshes = std::move(other.mMeshes);
        return *this;
    }

    const Matrix4f& RenderObject3D::getModelMatrix() {
        if (mDirty) {
            mModel = Matrix4f::Translation(mPosition) * Matrix4f(mRotation.toMatrix()) * Matrix4f::Scale(mScale);
            mDirty = false;
        }
        return mModel;
    }
} // Core