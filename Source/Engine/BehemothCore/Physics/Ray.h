#pragma once
#include "Math/MathCore.h"
#include "Collision/CollisionMask.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class ContactData;
	class CollisionData;

	class Ray
	{
	public:
		Ray();
		Ray(BMath::Vector3 position, BMath::Vector3 direction);

		BMath::Vector3 origin;
		BMath::Vector3 direction;
	};

	static bool RayCast(ECS::Registry& registry, const Ray& ray, ContactData& data,  BMask::CollisionType mask = BMask::CollisionType::AllCollision);
	static bool RayCast(ECS::Registry& registry, const Ray& ray, ContactData data[], BMask::CollisionType mask = BMask::CollisionType::AllCollision);
}