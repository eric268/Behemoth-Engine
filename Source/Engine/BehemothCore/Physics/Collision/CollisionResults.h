#pragma once
#include "Math/Vector.h"

namespace Behemoth
{
	struct ContactData
	{
		BMath::Vector3 collisionPoint;
		BMath::Vector3 collisionNormal;
		float penetrationDepth;
	};

	struct CollisionData
	{
		ContactData* contacts;
		int totalContacts;
	};
}
