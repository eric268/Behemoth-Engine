#pragma once

#include "Math/MathCore.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Physics/Collision/CollisionMask.h"
#include "Physics/Collision/Colliders.h"

#include <string>
#include <vector>

#define GRAVITY -9.81

namespace Behemoth
{
	class BVHNode;

	// Physics Components
	struct RigidBodyComponent : public ECS::Component
	{
		RigidBodyComponent() : affectedByGravity(true) {}
		RigidBodyComponent(bool gravity) : affectedByGravity(gravity) {}
		bool affectedByGravity;
	};

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

	struct BroadColliderComponent : public ColliderComponent
	{
		BroadColliderComponent(bool enabled = true) :
			ColliderComponent(enabled, BMask::CollisionType::BVHComponent, BMask::CollisionLayer::BVHCollider) {}

		AABBCollider collider;
		BMath::Vector3 extents;
	};


	struct OBBColliderComponent : public ColliderComponent
	{
		OBBColliderComponent(BMath::Vector3 extent = BMath::Vector3(1.0f), bool enabled = true,  BMask::CollisionType collisionType = BMask::CollisionType::DynamicType, BMask::CollisionLayer collisionLayer = BMask::CollisionLayer::Everything) :
			ColliderComponent(enabled, collisionType, collisionLayer),
			extents(extents)
		{}

		BMath::Vector3 extents;
		OBBCollider collider;
	};

	struct SphereColliderComponent : public ColliderComponent
	{
		SphereColliderComponent(BMath::Vector3 extent = BMath::Vector3(1.0f), bool enabled = true, BMask::CollisionType collisionType = BMask::CollisionType::StaticType, BMask::CollisionLayer collisionLayer = BMask::CollisionLayer::EnvObject) :
			ColliderComponent(enabled, collisionType, collisionLayer),
			extents(extents)
		{}

		BMath::Vector3 extents;
		SphereCollider collider;
	};

	struct NarrowCollider : public ColliderComponent
	{
		NarrowCollider(bool enabled = true, BMask::CollisionType collisionType = BMask::CollisionType::DynamicType, BMask::CollisionLayer collisionLayer = BMask::CollisionLayer::Everything) :
			ColliderComponent(enabled, collisionType, collisionLayer)
		{}

		std::vector<ECS::EntityHandle> colliderHandles;
		std::vector<OBBCollider> boxColliders;
		std::vector<SphereCollider> sphereColliders;
	};

	struct BroadCollisionPair : public ECS::Component
	{
		ECS::EntityHandle handle1;
	};

	struct CollisionDataComponent : public ECS::Component
	{
		BMath::Vector3 collisionPoint;
		BMath::Vector3 collisionNormal;
		float penetrationDepth;
		ECS::EntityHandle handle1;
		ECS::EntityHandle handle2;
		BMath::Vector3 relativeVelocity;
	};

	template<typename ...T>
	struct RequiredTypes
	{

	};

	template <typename ... T>
	struct BVHComponent : public ECS::Component
	{
		BVHComponent(std::shared_ptr<BVHNode> root = nullptr) : rootNode(root) {}
		std::shared_ptr<BVHNode> rootNode;
		RequiredTypes<T...> types;
	};
}