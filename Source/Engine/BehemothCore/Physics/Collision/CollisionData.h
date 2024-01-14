#pragma once
#include "Math/Vector.h"
#include "ECS/Entity.h"

#include <vector>

namespace Behemoth
{
	struct ContactData
	{
		ContactData() : collisionPoint(BMath::Vector3::Zero()), collisionNormal(BMath::Vector3::Zero()), penetrationDepth(0.0f) {}

		ContactData(const BMath::Vector3& point, const BMath::Vector3& normal, double depth) : 
			collisionPoint(point), 
			collisionNormal(normal),
			penetrationDepth(depth)
		{}

		BMath::Vector3 collisionPoint;
		BMath::Vector3 collisionNormal;
		double penetrationDepth;
	};

	struct CollisionData
	{
		CollisionData() = default;
		CollisionData(ContactData d, ECS::EntityHandle e, BMath::Vector3 v) : data(d), otherHandle(e), otherVelocity(v) {}
		ContactData data;
		ECS::EntityHandle otherHandle;
		BMath::Vector3 otherVelocity;
	};
}
