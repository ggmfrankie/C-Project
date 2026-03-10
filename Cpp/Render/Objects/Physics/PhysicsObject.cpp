//
// Created by ertls on 09.03.2026.
//

#include "PhysicsObject.hpp"
#include <algorithm>
#include <execution>

namespace Obj {
    PhysicsObject::PhysicsObject(const std::string& objFile, const std::initializer_list<Collider> list): Object(objFile) {
        for (auto& entry: list) {
            m_colliders << entry;
        }
    }

    PhysicsObject::PhysicsObject(PhysicsObject &&other) noexcept : Object(std::move(other)),
        m_colliders(std::move(other.m_colliders)),
        m_behavior(other.m_behavior) {

        other.m_behavior = nullptr;
        other.m_colliders.clear();
    }

    PhysicsObject::~PhysicsObject() {
        delete m_behavior;
    }

    void PhysicsObject::init(Render::Shader *s) {
        Object::init(s);
        updateColliders();
    }

    bool PhysicsObject::collidesWith(PhysicsObject& other) {
        for (auto& collider: m_colliders) {
            for (auto& otherCollider: other.m_colliders) {
                if (collider.isOverlap(otherCollider)) return true;
            }
        }
        return false;
    }

    void PhysicsObject::onCollision(PhysicsObject& other) {
        if (!m_behavior) return;
        m_behavior->onCollision(*this, other);
    }

    void PhysicsObject::onUpdate(double dt, Render::Input& input) {
        if (!m_behavior) return;
        m_behavior->onUpdate(*this, dt, input);
    }

    void PhysicsObject::updateColliders() {
        auto M = getModelMatrix();
        std::for_each(std::execution::seq, m_colliders.begin(), m_colliders.end(),
            [&M](Collider& other) {
                other.update(M);
            }
        );
    }

    void PhysicsObject::rotateBy(const float pitch, const float yaw, const float roll){
        Object::rotateBy(pitch, yaw, roll);
        updateColliders();
    }

    void PhysicsObject::moveBy(const float dx, const float dy, const float dz) {
        Object::moveBy(dx, dy, dz);
        updateColliders();
    }

    void PhysicsObject::moveTo(const float x, const float y, const float z) {
        Object::moveTo(x, y, z);
        updateColliders();
    }

    void PhysicsObject::attachBehavior(IBehavior* behavior) {
        m_behavior = behavior;
    }

    bool PhysicsObject::operator==(const PhysicsObject &physics_object) const {
        return uuid == physics_object.uuid;
    }

    PhysicsObject PhysicsObject::GetDummyPhysicsObject() {
        return {"grass_block\\grass_block.obj", {Collider({0,0,0}, 1, 1, 1, {0, 0, 0})}};
    }

} // Obj