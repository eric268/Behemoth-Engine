#pragma once

#include "Math/Vector.h"
#include "Geometry/VertexData.h"

#include <vector>

namespace Behemoth
{
	class AABBCollider;
	class SphereCollider;

	class BoundingGenerator
	{
	public:
		static bool GenerateAABBBoundingVolume(const std::vector<VertexData>& verticies, AABBCollider& collider);
		static bool GenerateSphereBoundingVolume(const std::vector<VertexData>& verticies, SphereCollider& collider);
	};
}
