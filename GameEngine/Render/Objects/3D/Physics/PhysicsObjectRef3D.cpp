//
// Created by Stefan on 15.03.2026.
//

#include "PhysicsObjectRef3D.hpp"

#include "Utils/ggmdef.hpp"
#include "Utils/Math/Quaternion.hpp"

namespace ggm {
    PhysicsObjectRef3D::PhysicsObjectRef3D(const JPH::BodyID ID, JPH::BodyInterface& bodyInterface) :
        mBodyID(ID),
        mBodyInterface(bodyInterface)
    {}

    void PhysicsObjectRef3D::applyForce(float x, float y, float z) const {
        mBodyInterface.AddForce(mBodyID, JPH::Vec3(x, y, z));
    }

    void PhysicsObjectRef3D::setPosition(float x, float y, float z) {
        mBodyInterface.SetPosition(mBodyID, JPH::Vec3(x, y, z), JPH::EActivation::Activate);
    }

    void PhysicsObjectRef3D::setRotation(float pitch, float yaw, float roll) {
        auto r = Quaternion::fromEuler(pitch, yaw, roll);
        mBodyInterface.SetRotation(mBodyID, {r.x, r.y, r.z, r.w}, JPH::EActivation::Activate);
    }

    Vector3f PhysicsObjectRef3D::getPosition() const {
        const auto v = mBodyInterface.GetPosition(mBodyID);
        return {v.GetX(), v.GetY(), v.GetZ()};
    }

    Quaternion PhysicsObjectRef3D::getRotation() const {
        const auto rot = mBodyInterface.GetRotation(mBodyID);
        return {rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ()};
    }
} // Obj