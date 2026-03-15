//
// Created by ertls on 09.03.2026.
//

#ifndef MIXEDPROJECT_PHYSICSOBJECT_HPP
#define MIXEDPROJECT_PHYSICSOBJECT_HPP
#include "Collider.hpp"
#include "../Render/RenderObject.hpp"
#include "Render/Objects/Behaviors/IBehavior.hpp"

namespace Obj {
    class PhysicsObject_old: public RenderObject {
        ggm::InlineVector<Collider, 16> m_colliders{};
        IBehavior* m_behavior = nullptr;

        ggm::Vector3f velocity{0,0,0};
        ggm::Vector3f acceleration{0,0,0};
        ggm::Vector3f forces{0,0,0};

        ggm::Vector3f angularVelocity = {0,0,0};
        ggm::Vector3f torques = {0,0,0};

        ggm::Matrix3f inertiaLocal;
        ggm::Matrix3f invInertiaLocal;

        ggm::Matrix3f inertiaWorld;
        ggm::Matrix3f invInertiaWorld;

        float restitution = 0.2f;
        float friction = 0.5f;

        float mass = 100.0;
        float invMass = 1/100.0;
        bool isStatic = false;

        void computeInertiaTensor();
        void updateColliders();

        void updateInertiaWorld();

        void resolveCollision(
            PhysicsObject_old& other,
            const ggm::Vector3f& normal,
            float penetration,
            const ggm::Vector3f& contactPoint
        );
    public:
        PhysicsObject_old(const std::string &objFile, std::initializer_list<Collider> list);
        PhysicsObject_old(PhysicsObject_old && other) noexcept ;

        ~PhysicsObject_old() override;

        void init(Render::Shader *s) override;

        bool collidesWith(PhysicsObject_old &other);

        void integrate(double dt);

        void onCollision(PhysicsObject_old &other);
        void update(double dt, Render::Input &input);

        void rotateBy(float pitch, float yaw, float roll) override;
        void rotateBy(const ggm::Vector3f &v);

        void moveBy(float dx, float dy, float dz) override;
        void moveTo(float x, float y, float z) override;

        void applyForce(const ggm::Vector3f &force);

        void attachBehavior(IBehavior *behavior);

        bool operator==(const PhysicsObject_old & physics_object) const;

        static PhysicsObject_old GetDummyPhysicsObject();
    };
} // Obj

#endif //MIXEDPROJECT_PHYSICSOBJECT_HPP