#pragma once

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Geometry/VertexData.h"

#include <vector>

namespace Behemoth
{
	struct AABBCollider;
	struct SphereCollider;

	class BoundingGenerator
	{
	public:
		static bool GenerateAABBBoundingVolume(const std::vector<VertexData>& verticies, AABBCollider& collider);
		static bool GenerateSphereBoundingVolume(const std::vector<VertexData>& verticies, SphereCollider& collider);
		static bool GenerateRotatedAABB(const AABBCollider& collider, const BMath::BMatrix3x3& rotationMatrix, AABBCollider& rotatedCollider);

	};
}
