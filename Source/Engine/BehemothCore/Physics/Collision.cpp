#include "pch.h"
#include "Collision.h"
#include "Components/Components.h"
#include "ECS/ECSCore.h"

namespace Behemoth
{
	bool CheckAABBCollision(const Math::Vector3& box1Min, const Math::Vector3& box1Max, const Math::Vector3& box2Min, const Math::Vector3& box2Max)
	{
		if (box1Max.x < box2Min.x || box1Min.x > box2Max.x)
			return false;
		if (box1Max.y < box2Min.y || box1Min.y > box2Max.y)
			return false;
		if (box1Max.z < box2Min.z || box1Min.z > box2Max.z)
			return false;

		return true;
	}

	bool CheckSphereSphereCollision(const Math::Vector3 sphere1Pos, const float sphere1Radius, const Math::Vector3 sphere2Pos, const float sphere2Radius)
	{
		float distance = Math::Vector3::SquaredDistance(sphere1Pos, sphere2Pos);
		float rad = sphere1Radius + sphere2Radius;
		return distance <= (rad * rad);
	}
}