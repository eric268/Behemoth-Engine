#pragma once

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Components/Components.h"
#include "Geometry/VertexData.h"
#include "Physics/Collision/Colliders.h"
#include "Components/PhysicsComponents.h"
#include "Misc/TransformHelper.h"

#include <vector>

namespace Behemoth
{
	struct BoundingGenerator
	{
		static bool GenerateAABBBoundingVolume(const std::vector<VertexData>& vertices, AABBCollider& collider);
		static bool GenerateSphereBoundingVolume(const std::vector<VertexData>& vertices, SphereCollider& collider);
		static bool GenerateRotatedAABB(const AABBCollider& collider, const BMath::Matrix3x3& rotationMatrix, AABBCollider& rotatedCollider);
	};

	template<typename T>
	static AABBCollider GenerateBVHCollider(TransformComponent* transformComp, const T* collider);

	template<>
	static AABBCollider GenerateBVHCollider(TransformComponent* transformComp, const AABBColliderComponent* collider)
	{
		AABBCollider bvhCollider{};
		bvhCollider.position = transformComp->worldPosition;
		bvhCollider.extents = transformComp->worldScale * collider->extents;
		return bvhCollider;
	}

	template<>
	static AABBCollider GenerateBVHCollider(TransformComponent* transformComp, const SphereColliderComponent* collider)
	{
		AABBCollider bvhCollider{};
		bvhCollider.position = transformComp->worldPosition;
		bvhCollider.extents = transformComp->worldScale * collider->radius;
		return bvhCollider;
	}

	template<>
	static AABBCollider GenerateBVHCollider(TransformComponent* transformComp, const OBBColliderComponent* collider)
	{
		AABBCollider bvhCollider{};
		bvhCollider.position = transformComp->worldPosition;
		bvhCollider.extents = transformComp->worldScale * collider->extents;

		BMath::Matrix3x3 rotationMatrix = TransformHelper::ExtractRotationMatrix(transformComp->worldTransform, transformComp->worldScale);
		AABBCollider rotatedCollider{};

		BoundingGenerator::GenerateRotatedAABB(bvhCollider, rotationMatrix, rotatedCollider);

		return rotatedCollider;
	}
}
