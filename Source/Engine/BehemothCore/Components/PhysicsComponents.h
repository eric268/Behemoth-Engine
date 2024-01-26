#pragma once

#include "Math/MathCore.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Physics/Collision/CollisionMask.h"
#include "Physics/Collision/Colliders.h"
#include "Physics/Collision/CollisionData.h"
#include "Physics/Collision/PhysicsMaterial.h"

#include <string>
#include <vector>

#define GRAVITY -9.81

namespace Behemoth
{
	struct BVHNode;

	// Physics Components
	struct RigidBodyComponent : public ECS::Component
	{
		RigidBodyComponent() : affectedByGravity(true), simulatePhysics(true) {}
		RigidBodyComponent(bool gravity, bool simPhysics) : affectedByGravity(gravity), simulatePhysics(simPhysics) {}
		bool affectedByGravity;
		bool simulatePhysics;
	};

	// Collider Components
	struct ColliderComponent : public ECS::Component
	{
	protected:
		ColliderComponent(
			bool isTrigger = false,
			bool enabled = true,
			BMask::CollisionType collisionType = BMask::CollisionType::StaticType,
			BMask::CollisionLayer collisionLayer = BMask::CollisionLayer::EnvObject)
			:
			isTrigger(isTrigger),
			isEnabled(enabled),
			collisionType(collisionType),
			collisionLayer(collisionLayer),
			physicsMaterial(PhysicsMaterial::None())
		{}
	public:
		bool isEnabled;
		bool isTrigger;
		BMask::CollisionType collisionType;
		BMask::CollisionLayer collisionLayer;
		PhysicsMaterial physicsMaterial;
	};
														
	// Broad collision components
	struct AABBColliderComponent : public ColliderComponent
	{
		AABBColliderComponent(
			BMath::Vector3 extents = BMath::Vector3(1.0f),
			bool enabled = true,
			bool isTrigger = false,
			BMask::CollisionType collisionType = BMask::CollisionType::StaticType,
			BMask::CollisionLayer collisionLayer = BMask::CollisionLayer::EnvObject)
			:
			ColliderComponent(isTrigger, enabled, collisionType, collisionLayer),
			extents(extents)
		{}

		AABBColliderComponent(
			const AABBCollider& collider,
			BMath::Vector3 extents = BMath::Vector3(1.0f),
			bool isTrigger = false,
			bool enabled = true)
			:
			ColliderComponent(isTrigger, enabled, collisionType, collisionLayer),
			collider(collider),
			extents(extents)
		{}
														
		BMath::Vector3 extents;
		AABBCollider collider;
	};

	struct BroadCollisionPairsComponent : public ECS::Component
	{
		BroadCollisionPairsComponent() {}
		BroadCollisionPairsComponent(std::vector<ECS::EntityHandle>& vec) : collisionIDs((vec)) {}

		std::vector<ECS::EntityHandle> collisionIDs;
	};

	// Narrow collision components
	struct OBBColliderComponent : public ColliderComponent
	{
		OBBColliderComponent(
			BMath::Vector3 extent = BMath::Vector3(1.0f),
			bool isTrigger = false,
			bool enabled = true,
			BMask::CollisionType collisionType = BMask::CollisionType::DynamicType,
			BMask::CollisionLayer collisionLayer = BMask::CollisionLayer::Everything)
			:
			ColliderComponent(isTrigger, enabled, collisionType, collisionLayer),
			extents(extent)
		{}

		BMath::Vector3 extents;
		OBBCollider collider;
	};

	struct SphereColliderComponent : public ColliderComponent
	{
		SphereColliderComponent(
			float radius = 1.0f,
			bool isTrigger = false,
			bool enabled = true,
			BMask::CollisionType collisionType = BMask::CollisionType::StaticType,
			BMask::CollisionLayer collisionLayer = BMask::CollisionLayer::EnvObject)
			:
			ColliderComponent(isTrigger, enabled, collisionType, collisionLayer),
			radius(radius)
		{}
		float radius;
		SphereCollider collider;
	};

	template <typename ...T>
	struct NarrowColliders {};
	using NarrowColliderTypes = NarrowColliders<AABBColliderComponent, SphereColliderComponent, OBBColliderComponent>;

	// Collision Resolution Components
	struct CollisionDataComponent : public ECS::Component
	{
		CollisionDataComponent() = default;
		std::vector<CollisionData> data;
	};

	struct TriggerDataComponent : public ECS::Component
	{
		TriggerDataComponent() = default;
		std::vector<CollisionData> data;

	};
	// Bounding Volume Hierarchy components
	template<typename ...T>
	struct RequiredTypes {};

	template <typename ... T>
	struct BVHRootComponent : public ECS::Component
	{
		BVHRootComponent(std::shared_ptr<BVHNode> root = nullptr) : rootNode(root) {}
		std::shared_ptr<BVHNode> rootNode;
		RequiredTypes<T...> types;
	};

	struct BVHColliderComponent : public ColliderComponent
	{
		BVHColliderComponent(bool enabled = true) :
			ColliderComponent(false, enabled, BMask::CollisionType::BVHComponent, BMask::CollisionLayer::BVHCollider),
			extents(1.0f)
		{}

		AABBCollider collider;
		BMath::Vector3 extents;
	};
}