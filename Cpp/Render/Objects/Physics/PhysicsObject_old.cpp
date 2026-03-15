//
// Created by ertls on 09.03.2026.
//

#include "PhysicsObject_old.hpp"
#include <algorithm>
#include <execution>

namespace Obj {
    PhysicsObject_old::PhysicsObject_old(const std::string& objFile, const std::initializer_list<Collider> list) : RenderObject(objFile) {
        for (auto &entry: list) {
            m_colliders << entry;
        }
    }

    PhysicsObject_old::PhysicsObject_old(PhysicsObject_old &&other) noexcept : RenderObject(std::move(other)),
        m_colliders(std::move(other.m_colliders)),
        m_behavior(other.m_behavior)
    {
        other.m_behavior = nullptr;
        other.m_colliders.clear();
    }

    PhysicsObject_old::~PhysicsObject_old() {
        delete m_behavior;
    }

    void PhysicsObject_old::init(Render::Shader *s) {
        RenderObject::init(s);
        updateColliders();
        computeInertiaTensor();
    }

    bool PhysicsObject_old::collidesWith(PhysicsObject_old& other) {
        for (auto& colliderA: m_colliders) {
            for (auto& colliderB: other.m_colliders) {
                if(auto [overlap, normal, penetration, point] = colliderA.isOverlap(colliderB); overlap) {
                    resolveCollision(other, normal, penetration, point);
                    return true;
                }
            }
        }
        return false;
    }

    void PhysicsObject_old::resolveCollision(PhysicsObject_old &other, const ggm::Vector3f &normal, const float penetration, const ggm::Vector3f &contactPoint) {
        using namespace ggm;

        if (isStatic && other.isStatic)
            return;

        // --- 1. Compute radii from centers to contact point
        Vector3f rA = contactPoint - this->position;
        Vector3f rB = contactPoint - other.position;

        // --- 2. Relative velocity at contact
        Vector3f vA = velocity + angularVelocity.cross(rA);
        Vector3f vB = other.velocity + other.angularVelocity.cross(rB);
        Vector3f rv = vB - vA;

        float velAlongNormal = rv.dot(normal);
        if (velAlongNormal > 0)
            return; // moving apart

        // --- 3. Compute restitution
        float e = std::min(this->restitution, other.restitution);

        // --- 4. Compute impulse scalar j
        float numerator = -(1.0f + e) * velAlongNormal;

        Vector3f crossA = rA.cross(normal);
        Vector3f crossB = rB.cross(normal);

        float denom =
            this->invMass +
            other.invMass +
            normal.dot(
                (this->invInertiaWorld * crossA).cross(rA) +
                (other.invInertiaWorld * crossB).cross(rB)
            );

        if (denom < 1e-6f) return;

        float j = numerator / denom*0.02f;

        Vector3f impulse = normal * j;

        // --- 5. Apply linear impulse
        if (!isStatic)
            velocity += impulse * invMass;
        if (!other.isStatic)
            other.velocity -= impulse * other.invMass;

        // --- 6. Apply angular impulse
        if (!isStatic)
            angularVelocity += invInertiaWorld * (rA.cross(impulse));
        if (!other.isStatic)
            other.angularVelocity -= other.invInertiaWorld * (rB.cross(impulse));

        // --- 7. Positional correction (to prevent sinking)
        const float k_slop = 0.01f;
        const float percent = 0.4f;

        float totalInvMass = invMass + other.invMass;
        if (totalInvMass <= 0.0f) return;

        float correctionMag = std::max(penetration - k_slop, 0.0f)
                              * percent;

        Vector3f correction = normal * (correctionMag / totalInvMass);

        if (!isStatic)
            moveBy(-correction.x * invMass, -correction.y * invMass, -correction.z * invMass);
        if (!other.isStatic)
            other.moveBy(correction.x * other.invMass, correction.y * other.invMass, correction.z * other.invMass);
    }

    void PhysicsObject_old::integrate(const double dt) {
        if (isStatic) return;

        acceleration = forces * (1.0f/mass);
        torques += torques.cross(forces);

        velocity += acceleration * static_cast<float>(dt);
        moveBy(velocity.x * static_cast<float>(dt), velocity.y * static_cast<float>(dt), velocity.z * static_cast<float>(dt));

        rotateBy(invInertiaWorld * angularVelocity * dt);
        angularVelocity *= 0.995f;

        dirty = true;
        forces = {};
    }

    void PhysicsObject_old::computeInertiaTensor() {
        using ggm::Matrix3f, ggm::Vector3f;

        if (isStatic || mass <= 0.f || m_colliders.empty()) {
            // Convention: zero inverse tensors → no rotation/torque response
            inertiaLocal     = Matrix3f::Identity();
            invInertiaLocal  = Matrix3f::Diagonal(0.f,0.f,0.f);
            inertiaWorld     = inertiaLocal;
            invInertiaWorld  = invInertiaLocal;
            return;
        }

        // 1) Mass distribution by collider volume (better than equal share)
        float totalVolume = 0.f;
        for (const auto& c : m_colliders) {
            const Vector3f he = c.localHalfSides;
            const float w = he.x * 2.f;
            const float h = he.y * 2.f;
            const float d = he.z * 2.f;
            totalVolume += (w * h * d);
        }
        if (totalVolume <= 1e-8f) {
            // degenerate shapes: guard
            totalVolume = 1.f;
        }

        // 2) Sum inertia contributions
        Matrix3f I_sum = Matrix3f::Diagonal(0.f,0.f,0.f);

        for (const auto& c : m_colliders) {
            const Vector3f he = c.localHalfSides;
            const float w = he.x * 2.f;
            const float h = he.y * 2.f;
            const float d = he.z * 2.f;

            const float volume = w * h * d;
            const float mc = mass * (volume / totalVolume);

            // (a) box inertia in collider's local principal frame (about collider center)
            const float Ixx = (mc / 12.f) * (h*h + d*d);
            const float Iyy = (mc / 12.f) * (w*w + d*d);
            const float Izz = (mc / 12.f) * (w*w + h*h);
            const Matrix3f I_box_diag = Matrix3f::Diagonal(Ixx, Iyy, Izz);

            // (b) collider's orientation in object-local space
            // Use your stored local axes (should be unit vectors)
            const Vector3f ex = Vector3f::normalize(c.localAxisX);
            const Vector3f ey = Vector3f::normalize(c.localAxisY);
            const Vector3f ez = Vector3f::normalize(c.localAxisZ);
            const Matrix3f Rc = Matrix3f::FromColumns(ex, ey, ez);

            // I_rot = Rc * I_box_diag * Rc^T
            const Matrix3f I_rot = (Rc * I_box_diag) * Rc.transpose();

            // (c) parallel axis: shift to object origin
            const Vector3f r = c.localCenter;        // collider center relative to object origin
            const float r2 = r.dot(r);
            const Matrix3f par = Matrix3f::Identity() * r2 - Matrix3f::Outer(r, r);

            const Matrix3f I_shift = I_rot + par * mc;

            // (d) accumulate
            I_sum = I_sum + I_shift;
        }

        inertiaLocal = I_sum;

        // 3) Inverse in local space
        invInertiaLocal = inertiaLocal.inverse();  // your Matrix3f::inverse() for column-major

        // 4) Precompute world-space tensors (optional here; else do it per-frame using current rotation)
        // R = object rotation (3x3) – must be orthonormal (no non-uniform scale)
        // If your object has non-uniform scale, do NOT do R*I*R^T; recompute I from scaled geometry instead.
        const ggm::Matrix4f model = this->getModelMatrix();
        // Extract upper-left 3x3 rotation (you should implement this extractor)
        const Matrix3f R = model.toRotationMatrix(); // <-- implement: returns orthonormal 3x3

        inertiaWorld    = (R * inertiaLocal) * R.transpose();
        invInertiaWorld = inertiaWorld.inverse();

    }

    void PhysicsObject_old::onCollision(PhysicsObject_old& other) {
        if (!m_behavior) return;
        m_behavior->onCollision(*this, other);
    }

    void PhysicsObject_old::update(const double dt, Render::Input& input) {
        if (dirty) {
            updateColliders();
            updateInertiaWorld();
        }
        if (m_behavior) m_behavior->onUpdate(*this, dt, input);

        RenderObject::update();
    }

    void PhysicsObject_old::updateColliders() {
        auto M = getModelMatrix();
        std::for_each(std::execution::seq, m_colliders.begin(), m_colliders.end(),
            [&M](Collider& other) {
                other.update(M);
            }
        );
    }

    void PhysicsObject_old::updateInertiaWorld() {
        const ggm::Matrix3f R = getModelMatrix().toRotationMatrix();
        inertiaWorld    = (R * inertiaLocal) * R.transpose();
        invInertiaWorld = inertiaWorld.inverse();
    }

    void PhysicsObject_old::rotateBy(const float pitch, const float yaw, const float roll){
        RenderObject::rotateBy(pitch, yaw, roll);
        dirty = true;
    }

    void PhysicsObject_old::rotateBy(const ggm::Vector3f& v){
        RenderObject::rotateBy(v.x, v.y, v.z);
        dirty = true;
    }

    void PhysicsObject_old::moveBy(const float dx, const float dy, const float dz) {
        RenderObject::moveBy(dx, dy, dz);
        dirty = true;
    }

    void PhysicsObject_old::moveTo(const float x, const float y, const float z) {
        RenderObject::moveTo(x, y, z);
        dirty = true;
    }

    void PhysicsObject_old::applyForce(const ggm::Vector3f& force) {
        forces += force;
    }

    void PhysicsObject_old::attachBehavior(IBehavior* behavior) {
        m_behavior = behavior;
    }

    bool PhysicsObject_old::operator==(const PhysicsObject_old &physics_object) const {
        return uuid == physics_object.uuid;
    }

    PhysicsObject_old PhysicsObject_old::GetDummyPhysicsObject() {
        return {"grass_block\\grass_block.obj", {Collider({0,0,0}, 1, 1, 1, {0, 0, 0})}};
    }
} // Obj