#pragma once
#include "Math/Vector.h"

namespace Behemoth
{
	struct ContactData
	{
		Math::Vector3 collisionPoint;
		Math::Vector3 collisionNormal;
		float penetrationDepth;
	};

	struct CollisionData
	{
		ContactData* contacts;
		int totalContacts;
	};
}
