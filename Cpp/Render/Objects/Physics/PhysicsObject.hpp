//
// Created by ertls on 09.03.2026.
//

#ifndef MIXEDPROJECT_PHYSICSOBJECT_HPP
#define MIXEDPROJECT_PHYSICSOBJECT_HPP
#include "Collider.hpp"
#include "Render/Objects/Object.hpp"
#include "Render/Objects/Behaviors/IBehavior.hpp"

namespace Obj {
    class PhysicsObject: public Object {
        ggm::InlineVector<Collider, 16> m_colliders{};
        IBehavior* m_behavior{};
    public:
        PhysicsObject(const std::string &objFile, std::initializer_list<Collider> list);
        ~PhysicsObject() override;

        void init(Render::Shader *s) override;

        bool collidesWith(PhysicsObject &other);

        void updateColliders();

        void rotateBy(float pitch, float yaw, float roll) override;

        void moveBy(float dx, float dy, float dz) override;

        void moveTo(float x, float y, float z) override;

        void attachBehavior(IBehavior *behavior);

        static PhysicsObject GetDummyPhysicsObject();
    };
} // Obj

#endif //MIXEDPROJECT_PHYSICSOBJECT_HPP