#pragma once
#include "Math/MathCore.h"
#include "ECS/Component.h"
#include "Physics/CollisionMask.h"
#include "Physics/Colliders.h"

#include <string>

namespace Behemoth
{
	// Physics Components
	struct RigidBodyComponent : public ECS::Component
	{
		RigidBodyComponent() {}
	};

	struct ColliderComponent : public ECS::Component
	{
	protected:
		ColliderComponent(Collision::CollisionMask collisionType = Collision::CollisionMask::EnvObject, bool enabled = true) :
			collisionType(collisionType),
			isEnabled(enabled),
			collisionMask(Behemoth::Collision::CollisionMask::Everything) {}
	public:
		bool isEnabled;
		std::uint16_t collisionType;
		std::uint16_t collisionMask;
	};

	struct AABBColliderComponent : public ColliderComponent
	{
		AABBColliderComponent(Math::Vector3 extents, Collision::CollisionMask collisionType = Collision::CollisionMask::EnvObject, bool enabled = true) :
			ColliderComponent(collisionType, enabled),
			collider(extents/2.0f)
		{}

		Collision::AABBCollider collider;
	};

	struct OBBColliderComponent : public ColliderComponent
	{
		OBBColliderComponent(Math::Vector3 extent, Collision::CollisionMask collisionType = Collision::CollisionMask::EnvObject, bool enabled = true) :
			ColliderComponent(collisionType, enabled),
			collider(extent)
		{}
		
		Collision::OBBCollider collider;
	};

	struct SphereColliderComponent : public ColliderComponent
	{
		SphereColliderComponent(float radius, Collision::CollisionMask collisionType = Collision::CollisionMask::EnvObject, bool enabled = true) :
			ColliderComponent(collisionType, enabled),
			collider(radius)
		{}

		Collision::SphereCollider collider;
	};

	struct MeshColliderComponent : public ColliderComponent
	{
		MeshColliderComponent(std::string& filename, Math::Vector3 scale = Math::Vector3::One(), Behemoth::Collision::CollisionMask collisionType = Behemoth::Collision::CollisionMask::EnvObject, bool enabled = true) :
			ColliderComponent(collisionType, enabled),
			modelFileName(filename),
			// mesh(filename),
			scale(scale) {}

		Math::Vector3 scale;
		std::string modelFileName;
		// Mesh mesh;
	};
}