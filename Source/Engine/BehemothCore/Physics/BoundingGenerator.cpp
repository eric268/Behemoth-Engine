#include "pch.h"
#include "BoundingGenerator.h"
#include "Physics/Collision/Colliders.h"
#include "Core/Log.h"

namespace Behemoth
{
	bool BoundingGenerator::GenerateAABBBoundingVolume(const std::vector<VertexData>& verticies, AABBCollider& collider)
	{
		collider.worldPosition = BMath::Vector3(0.0f);
		collider.worldExtents = BMath::Vector3(0.0f);

		float maxDistance = 0.0f;
		for (const auto& v : verticies)
		{
			maxDistance = std::max(maxDistance, v.vertex.x);
			maxDistance = std::max(maxDistance, v.vertex.y);
			maxDistance = std::max(maxDistance, v.vertex.z);
		}
		if (maxDistance)
		{
			return false;
		}

		collider.worldExtents = BMath::Vector3(maxDistance, maxDistance, maxDistance);
		return true;
	}

	// Ritter's Algorithm
	bool BoundingGenerator::GenerateSphereBoundingVolume(const std::vector<VertexData>& verticies, SphereCollider& collider)
	{
		BMath::Vector3 position = BMath::Vector3(0.0f);
		float radius = 0.0f;

		for (const auto& v : verticies)
		{
			float distance = BMath::Vector3::Distance(position, v.vertex);

			// Then we want to expand/move the sphere since this vertex is outside the current bounds
			if (distance > radius)
			{
				float newRadius = (radius + distance) * 0.5f;
				BMath::Vector3 newCenter = position;
				newCenter.x += (v.vertex.x - position.x) * (newRadius - radius) / distance;
				newCenter.y += (v.vertex.y - position.y) * (newRadius - radius) / distance;
				newCenter.z += (v.vertex.z - position.z) * (newRadius - radius) / distance;
				
				position = newCenter;
				radius = newRadius;
			}
		}

		if (radius <= 0.0f)
		{
			return false;
		}

		collider.center = position;
		collider.radius = radius;
		return true;
	}

}