//
// Created by Stefan on 15.03.2026.
//

#include "PhysicsHandler3D.hpp"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include <unordered_map>

#include "Render/Objects/Physics/PhysicsFactory.hpp"

namespace Game {
    using namespace JPH;

    PhysicsHandler3D::PhysicsHandler3D() :
        mTempAllocator(10 * 1024 * 1024),
        mJobSystem(cMaxPhysicsJobs, cMaxPhysicsBarriers, static_cast<int>(thread::hardware_concurrency()) - 1)
    {
    }

    PhysicsHandler3D::~PhysicsHandler3D() = default;

    void PhysicsHandler3D::init() {
        mPhysicsSystem.Init(
            cMaxBodies,
            cNumBodyMutexes,
            cMaxBodyPairs,
            cMaxContactConstraints,
            mLayerInterface,
            mObjectVsBroadPhaseFilter,
            mObjectCollisionFilter
        );

        mPhysicsSystem.SetGravity(Vec3(0.0f, -9.81f, 0.0f));
        mPhysicsSystem.OptimizeBroadPhase();
    }

    void PhysicsHandler3D::update(const float dt) {
        mPhysicsSystem.Update(dt, 1, &mTempAllocator, &mJobSystem);
    }

    PhysicsSystem& PhysicsHandler3D::getPhysicsSystem() {
        return mPhysicsSystem;
    }

    Obj3D::PhysicsObjectRef PhysicsHandler3D::newObj(const Obj3D::PhysicsSettings &s) {
        const BodyCreationSettings settings {
            s.shape,
            Vec3(s.pos.x, s.pos.y, s.pos.z),
            Quat::sIdentity(),
            s.motionType,
            (s.motionType == EMotionType::Dynamic) ? Layers::MOVING : Layers::NON_MOVING
        };
        auto& interface = mPhysicsSystem.GetBodyInterface();
        BodyID id = interface.CreateAndAddBody(
            settings,
            (s.motionType == EMotionType::Dynamic) ? EActivation::Activate : EActivation::DontActivate
        );
        return {id, interface};
    }

    PhysicsHandler3D::BPLayerInterfaceImpl::BPLayerInterfaceImpl() {
        // Create a mapping table from object to broad phase layer
        mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
        mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;

    }

    bool PhysicsHandler3D::ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const {
        switch (inLayer1)
        {
            case Layers::NON_MOVING:
                return inLayer2 == BroadPhaseLayers::MOVING;
            case Layers::MOVING:
                return true;
            default:
                JPH_ASSERT(false);
                return false;
        }
    }

    bool PhysicsHandler3D::ObjectLayerPairFilterImpl::ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) const {
        switch (inObject1)
        {
            case Layers::NON_MOVING:
                return inObject2 == Layers::MOVING; // Non moving only collides with moving
            case Layers::MOVING:
                return true; // Moving collides with everything
            default:
                JPH_ASSERT(false);
                return false;
        }
    }

    ggm::u32 PhysicsHandler3D::BPLayerInterfaceImpl::GetNumBroadPhaseLayers() const {
        return BroadPhaseLayers::NUM_LAYERS;
    }

    BroadPhaseLayer PhysicsHandler3D::BPLayerInterfaceImpl::GetBroadPhaseLayer(ObjectLayer inLayer) const {
        JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
        return mObjectToBroadPhase[inLayer];
    }

    const char * PhysicsHandler3D::BPLayerInterfaceImpl::GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const {
        switch (static_cast<BroadPhaseLayer::Type>(inLayer))
        {
            case static_cast<BroadPhaseLayer::Type>(BroadPhaseLayers::NON_MOVING):	return "NON_MOVING";
            case static_cast<BroadPhaseLayer::Type>(BroadPhaseLayers::MOVING):		return "MOVING";
            default: JPH_ASSERT(false); return "INVALID";
        }
    }
} // Game