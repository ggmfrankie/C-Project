//
// Created by ertls on 09.03.2026.
//

#ifndef MIXEDPROJECT_OBJECTBEHAVOIR_HPP
#define MIXEDPROJECT_OBJECTBEHAVOIR_HPP

namespace Render {
    class Input;
}

namespace Obj3D {
    class PhysicsObject_old;

    class IBehavior {
    public:
        virtual ~IBehavior() = default;
        virtual void onCollision(PhysicsObject_old& self, PhysicsObject_old& other) = 0;
        virtual void onUpdate(PhysicsObject_old& self, double dt, Render::Input& input) = 0;
    };
} // Obj

#endif //MIXEDPROJECT_OBJECTBEHAVOIR_HPP