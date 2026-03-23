//
// Created by Stefan on 15.03.2026.
//

#include "PhysicsObject.hpp"

#include "Utils/Math/ggmdef.hpp"
#include "Utils/Math/Quaternion.hpp"

namespace Obj {
    PhysicsObject::PhysicsObject(const JPH::BodyID ID, JPH::BodyInterface& bodyInterface) :
        mBodyInterface(bodyInterface),
        mBodyID(ID)
    {}

    PhysicsObject::PhysicsObject(const PhysicsObject &&other) noexcept :
        mBodyID(other.mBodyID),
        mBodyInterface(other.mBodyInterface)
    {}

    void PhysicsObject::applyForce(float x, float y, float z) const {
        mBodyInterface.AddForce(mBodyID, JPH::Vec3(x, y, z));
    }

    void PhysicsObject::setPosition(float x, float y, float z) {
        mBodyInterface.SetPosition(mBodyID, JPH::Vec3(x, y, z), JPH::EActivation::Activate);
    }

    void PhysicsObject::setRotation(float pitch, float yaw, float roll) {
        auto r = ggm::Quaternion::fromEuler(pitch, yaw, roll);
        mBodyInterface.SetRotation(mBodyID, {r.x, r.y, r.z, r.w}, JPH::EActivation::Activate);
    }

    ggm::Vector3f PhysicsObject::getPosition() const {
        const auto v = mBodyInterface.GetPosition(mBodyID);
        return {v.GetX(), v.GetY(), v.GetZ()};
    }

    ggm::Quaternion PhysicsObject::getRotation() const {
        const auto rot = mBodyInterface.GetRotation(mBodyID);
        return {rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ()};
    }
} // Obj