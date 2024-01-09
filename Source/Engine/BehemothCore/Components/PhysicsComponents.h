#pragma once

#include "Math/MathCore.h"
#include "ECS/Component.h"
#include "Physics/Collision/CollisionMask.h"
#include "Physics/Collision/Colliders.h"

#include <string>

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
		ColliderComponent(BMath::Vector3 offset = BMath::Vector3(1.0f), CollisionMask collisionType = CollisionMask::EnvObject, bool enabled = true) :
			localOffset(offset),
			collisionType(collisionType),
			isEnabled(enabled),
			collisionMask(Behemoth::CollisionMask::Everything) {}
	public:
		bool isEnabled;
		BMath::Vector3 localOffset;
		std::uint16_t collisionType;
		std::uint16_t collisionMask;
	};

	struct AABBColliderComponent : public ColliderComponent
	{
		AABBColliderComponent(BMath::Vector3 offset = BMath::Vector3(1.0f), BMath::Vector3 extents = BMath::Vector3(1.0f), CollisionMask collisionType = CollisionMask::EnvObject, bool enabled = true) :
			ColliderComponent(offset, collisionType, enabled)
		{}

/*		BMath::Vector3 localExtents;*/
		AABBCollider collider;
	};

	struct OBBColliderComponent : public ColliderComponent
	{
		OBBColliderComponent(BMath::Vector3 offset = BMath::Vector3(1.0f), BMath::Vector3 extent = BMath::Vector3(1.0f), CollisionMask collisionType = CollisionMask::EnvObject, bool enabled = true) :
			ColliderComponent(offset, collisionType, enabled)
		{}

// 		BMath::Vector3 localeExtents;
// 		BMath::Quaternion localOrientation;
		OBBCollider collider;
	};

	struct SphereColliderComponent : public ColliderComponent
	{
		SphereColliderComponent(BMath::Vector3 offset = BMath::Vector3(1.0f), float radius = 1.0f, CollisionMask collisionType = CollisionMask::EnvObject, bool enabled = true) :
			ColliderComponent(offset, collisionType, enabled)
		{}

		// float localRadius;
		SphereCollider collider;
	};

// 	struct MeshColliderComponent : public ColliderComponent
// 	{
// 		MeshColliderComponent(BMath::Vector3 offset = BMath::Vector3(1.0f), std::string& filename, BMath::Vector3 scale = BMath::Vector3::One(), Behemoth::CollisionMask collisionType = Behemoth::CollisionMask::EnvObject, bool enabled = true) :
// 			ColliderComponent(offset, collisionType, enabled),
// 			modelFileName(filename),
// 			// mesh(filename),
// 			scale(scale) {}
// 
// 		BMath::Vector3 scale;
// 		std::string modelFileName;
// 		// Mesh mesh;
// 	};

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