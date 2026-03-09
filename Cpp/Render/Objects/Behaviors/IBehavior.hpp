//
// Created by ertls on 09.03.2026.
//

#ifndef MIXEDPROJECT_OBJECTBEHAVOIR_HPP
#define MIXEDPROJECT_OBJECTBEHAVOIR_HPP

namespace Render {
    class Input;
}

namespace Obj {
    class PhysicsObject;

    class IBehavior {
    public:
        virtual ~IBehavior() = default;
        virtual void onCollision(PhysicsObject& self, PhysicsObject& other) = 0;
        virtual void onUpdate(PhysicsObject& self, float dt, Render::Input& input) = 0;
    };
} // Obj

#endif //MIXEDPROJECT_OBJECTBEHAVOIR_HPP