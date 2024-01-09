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
		ColliderComponent(CollisionMask collisionType = CollisionMask::EnvObject, bool enabled = true) :
			collisionType(collisionType),
			isEnabled(enabled),
			collisionMask(Behemoth::CollisionMask::Everything) {}
	public:
		bool isEnabled;
		std::uint16_t collisionType;
		std::uint16_t collisionMask;
	};

	struct AABBColliderComponent : public ColliderComponent
	{
		AABBColliderComponent(BMath::Vector3 extents, BMath::Vector3 offset = BMath::Vector3::Zero(), CollisionMask collisionType = CollisionMask::EnvObject, bool enabled = true) :
			ColliderComponent(collisionType, enabled),
			localPositionOffset(offset),
			collider(extents)
		{}

		AABBCollider collider;
		BMath::Vector3 localPositionOffset;
		BMath::Vector3 localScale;
	};

	struct OBBColliderComponent : public ColliderComponent
	{
		OBBColliderComponent(BMath::Vector3 extent, BMath::Matrix4x4 offset = BMath::Matrix4x4::Identity(), CollisionMask collisionType = CollisionMask::EnvObject, bool enabled = true) :
			ColliderComponent(collisionType, enabled),
			localTransformOffset(offset),
			collider(extent)
		{}
		
		OBBCollider collider;
		BMath::Matrix4x4 localTransformOffset;
	};

	struct SphereColliderComponent : public ColliderComponent
	{
		SphereColliderComponent(float radius, BMath::Vector3 offset = BMath::Vector3::Zero(), CollisionMask collisionType = CollisionMask::EnvObject, bool enabled = true) :
			ColliderComponent(collisionType, enabled),
			localPositionOffset(offset),
			collider(radius)
		{}

		SphereCollider collider;
		BMath::Vector3 localPositionOffset;
		BMath::Vector3 localScale;
	};

	struct MeshColliderComponent : public ColliderComponent
	{
		MeshColliderComponent(std::string& filename, BMath::Vector3 scale = BMath::Vector3::One(), Behemoth::CollisionMask collisionType = Behemoth::CollisionMask::EnvObject, bool enabled = true) :
			ColliderComponent(collisionType, enabled),
			modelFileName(filename),
			// mesh(filename),
			scale(scale) {}

		BMath::Vector3 scale;
		std::string modelFileName;
		// Mesh mesh;
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