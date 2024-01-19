#pragma once

#include "Math/MathCore.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Physics/Collision/CollisionMask.h"
#include "Physics/Collision/Colliders.h"
#include "Physics/Collision/CollisionData.h"

#include <string>
#include <vector>

#define GRAVITY -9.81

namespace Behemoth
{
	struct BVHNode;

	// Physics Components
	struct RigidBodyComponent : public ECS::Component
	{
		RigidBodyComponent() : affectedByGravity(true) {}
		RigidBodyComponent(bool gravity) : affectedByGravity(gravity) {}
		bool affectedByGravity;
	};

	// Collider Components
	struct ColliderComponent : public ECS::Component
	{
	protected:
		ColliderComponent(bool enabled = true, BMask::CollisionType collisionType = BMask::CollisionType::StaticType, BMask::CollisionLayer collisionLayer = BMask::CollisionLayer::EnvObject) :
			isEnabled(enabled),
			collisionType(collisionType),
			collisionLayer(collisionLayer) {}
	public:
		bool isEnabled;
		BMask::CollisionType collisionType;
		BMask::CollisionLayer collisionLayer;
	};

	// Broad collision components
	struct AABBColliderComponent : public ColliderComponent
	{
		AABBColliderComponent(BMath::Vector3 extents = BMath::Vector3(1.0f), bool enabled = true, BMask::CollisionType collisionType = BMask::CollisionType::StaticType, BMask::CollisionLayer collisionLayer = BMask::CollisionLayer::EnvObject) :
			ColliderComponent(enabled, collisionType, collisionLayer),
			extents(extents)
		{}

		AABBColliderComponent(const AABBCollider& collider, BMath::Vector3 extents = BMath::Vector3(1.0f), bool enabled = true) :
			ColliderComponent(enabled, collisionType, collisionLayer),
			collider(collider),
			extents(extents) {}

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
		OBBColliderComponent(BMath::Vector3 extent = BMath::Vector3(1.0f), bool enabled = true,  BMask::CollisionType collisionType = BMask::CollisionType::DynamicType, BMask::CollisionLayer collisionLayer = BMask::CollisionLayer::Everything) :
			ColliderComponent(enabled, collisionType, collisionLayer),
			extents(extent)
		{}

		BMath::Vector3 extents;
		OBBCollider collider;
	};

	struct SphereColliderComponent : public ColliderComponent
	{
		SphereColliderComponent(float radius = 1.0f, bool enabled = true, BMask::CollisionType collisionType = BMask::CollisionType::StaticType, BMask::CollisionLayer collisionLayer = BMask::CollisionLayer::EnvObject) :
			ColliderComponent(enabled, collisionType, collisionLayer),
			radius(radius)
		{}
		float radius;
		SphereCollider collider;
	};

	template <typename ...T>
	struct NarrowColliders {};
	using NarrowColliderTypes = NarrowColliders<SphereColliderComponent, OBBColliderComponent>;

	// Collision Resolution Components
	struct CollisionDataComponent : public ECS::Component
	{
		CollisionDataComponent() = default;
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
			ColliderComponent(enabled, BMask::CollisionType::BVHComponent, BMask::CollisionLayer::BVHCollider),
			extents(1.0f)
		{}

		AABBCollider collider;
		BMath::Vector3 extents;
	};
}