//
// Created by Stefan on 15.03.2026.
//

#pragma once
#include "EngineDefines.hpp"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include "Render/Objects/3D/Physics/PhysicsObjectRef.hpp"
#include "Utils/ggmdef.hpp"

namespace Obj3D {
    struct PhysicsSettings;
}

namespace Game {
    namespace Layers
    {
        static constexpr JPH::ObjectLayer NON_MOVING = 0;
        static constexpr JPH::ObjectLayer MOVING = 1;
        static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
    };

    namespace BroadPhaseLayers
    {
        static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
        static constexpr JPH::BroadPhaseLayer MOVING(1);
        static constexpr ggm::u32 NUM_LAYERS(2);
    }

    class PhysicsHandler3D  {


        class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
            JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS]{};
        public:
            [[nodiscard]] ggm::u32 GetNumBroadPhaseLayers() const override;
            [[nodiscard]] JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

        #if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
            [[nodiscard]] const char * GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
        #endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

            BPLayerInterfaceImpl();
        };

        struct ObjectVsBroadPhaseLayerFilterImpl final : JPH::ObjectVsBroadPhaseLayerFilter {
            bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
        };

        struct ObjectLayerPairFilterImpl final : JPH::ObjectLayerPairFilter {
            bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
        };

        JPH::PhysicsSystem mPhysicsSystem;

        JPH::TempAllocatorImpl mTempAllocator;

        const ggm::u32 cMaxBodies = 1024;
        const ggm::u32 cNumBodyMutexes = 0;
        const ggm::u32 cMaxBodyPairs = 1024;
        const ggm::u32 cMaxContactConstraints = 1024;

        BPLayerInterfaceImpl mLayerInterface;
        ObjectLayerPairFilterImpl mObjectCollisionFilter;
        ObjectVsBroadPhaseLayerFilterImpl mObjectVsBroadPhaseFilter;

        JPH::JobSystemThreadPool mJobSystem;

    public:
        explicit PhysicsHandler3D();
        ~PhysicsHandler3D();

        void update(float dt);
        void init() ;

        JPH::PhysicsSystem& getPhysicsSystem();
        Obj3D::PhysicsObjectRef newObject(const Obj3D::PhysicsSettings& s);
    };
} // Game

