//
// Created by Stefan on 15.03.2026.
//

#pragma once
#include <Jolt/Jolt.h>
#include "Games/IGameLayer.hpp"
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/PhysicsSystem.h>


#include "Jolt/Core/JobSystemThreadPool.h"
#include "Render/Objects/Physics/PhysicsObject.hpp"
#include "Utils/Math/ggmdef.hpp"

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

    class PhysicsHandler final : public IGameLayer {


        class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
            JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS]{};
            ggm::u32 GetNumBroadPhaseLayers() const override;
            JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

        #if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
            [[nodiscard]] const char * GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
        #endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

        public:
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
        explicit PhysicsHandler();
        ~PhysicsHandler() override;

        void onAttach(const LayerEngineContext &ec) override;
        void onDetach() override;
        void onUpdate(float dt) override;
        void onRender(int width, int height) override;
        void onInit() override;

        Obj::PhysicsObject newBox(float x, float y, float z, const ggm::Vector3f &pos);
    };
} // Game

