#pragma once
#include "Math/Vector.h"

#include <vector>

namespace Behemoth
{
	struct ContactData
	{
		ContactData() : collisionPoint(BMath::Vector3::Zero()), collisionNormal(BMath::Vector3::Zero()), penetrationDepth(0.0f) {}

		ContactData(const BMath::Vector3& point, const BMath::Vector3& normal, float depth) : 
			collisionPoint(point), 
			collisionNormal(normal),
			penetrationDepth(depth)
		{}

		BMath::Vector3 collisionPoint;
		BMath::Vector3 collisionNormal;
		float penetrationDepth;
	};

	struct CollisionData
	{
		std::vector<ContactData> collisionContacts;
	};
}
