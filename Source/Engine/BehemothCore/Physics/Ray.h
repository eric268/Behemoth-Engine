#pragma once

#include "Math/MathCore.h"
#include "ECS/Entity.h"
#include "Collision/CollisionMask.h"
#include "Physics/Collision/CollisionData.h"


namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class Ray
	{
	public:
		Ray();
		Ray(BMath::Vector3 position, BMath::Vector3 direction);

		BMath::Vector3 origin;
		BMath::Vector3 direction;
	};

	bool RayCast(ECS::Registry& registry, const Ray& ray, std::vector<ContactData>& data, const std::vector<ECS::EntityHandle>& entitiesToIgnore, BMask::CollisionType mask = BMask::CollisionType::AllCollision);
	bool RayCast(ECS::Registry& registry, const Ray& ray, ContactData& data, const std::vector<ECS::EntityHandle>& entitiesToIgnore, BMask::CollisionType mask = BMask::CollisionType::AllCollision);
}