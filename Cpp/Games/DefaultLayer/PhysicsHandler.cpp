//
// Created by Stefan on 15.03.2026.
//

#include "PhysicsHandler.hpp"
#include "Jolt/RegisterTypes.h"
#include "Jolt/Core/Factory.h"

namespace Game {
    using namespace JPH;

    PhysicsHandler::PhysicsHandler() :
        mTempAllocator(10 * 1024 * 1024),
        mJobSystem(cMaxPhysicsJobs, cMaxPhysicsBarriers, static_cast<int>(thread::hardware_concurrency()) - 1) {
    }

    PhysicsHandler::~PhysicsHandler() = default;

    void PhysicsHandler::onAttach() {
        RegisterDefaultAllocator();
        Factory::sInstance = new Factory();
        RegisterTypes();

        mPhysicsSystem.Init(
            cMaxBodies,
            cNumBodyMutexes,
            cMaxBodyPairs,
            cMaxContactConstraints,
            mLayerInterface,
            mObjectVsBroadPhaseFilter,
            mObjectCollisionFilter
        );

        mPhysicsSystem.OptimizeBroadPhase();
    }

    void PhysicsHandler::onDetach() {
    }

    void PhysicsHandler::onUpdate(float dt) {
        mPhysicsSystem.Update(dt, 1, &mTempAllocator, &mJobSystem);
    }

    void PhysicsHandler::onRender() {
    }

    void PhysicsHandler::addObject(Obj::PhysicsObject &&obj) {
        mObjects.push_back(std::move(obj));
    }

    void PhysicsHandler::removeObject(Obj::PhysicsObject &obj) {
    }

    PhysicsHandler::BPLayerInterfaceImpl::BPLayerInterfaceImpl() {
        // Create a mapping table from object to broad phase layer
        mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
        mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;

    }

    bool PhysicsHandler::ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const {
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

    bool PhysicsHandler::ObjectLayerPairFilterImpl::ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) const {
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

    ggm::u32 PhysicsHandler::BPLayerInterfaceImpl::GetNumBroadPhaseLayers() const {
        return BroadPhaseLayers::NUM_LAYERS;
    }

    BroadPhaseLayer PhysicsHandler::BPLayerInterfaceImpl::GetBroadPhaseLayer(ObjectLayer inLayer) const {
        JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
        return mObjectToBroadPhase[inLayer];
    }

    const char * PhysicsHandler::BPLayerInterfaceImpl::GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const {
        switch (static_cast<BroadPhaseLayer::Type>(inLayer))
        {
            case static_cast<BroadPhaseLayer::Type>(BroadPhaseLayers::NON_MOVING):	return "NON_MOVING";
            case static_cast<BroadPhaseLayer::Type>(BroadPhaseLayers::MOVING):		return "MOVING";
            default:													JPH_ASSERT(false); return "INVALID";
        }
    }
} // Game