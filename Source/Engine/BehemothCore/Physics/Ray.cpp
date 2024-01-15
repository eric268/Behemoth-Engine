#include "pch.h"
#include "Ray.h"
#include "ECS/Registry.h"
#include "Components/PhysicsComponents.h"
#include "Components/Components.h"

namespace Behemoth
{
	bool BroadRayCheck(ECS::Registry& registry, const Ray& ray, ContactData& data, BMask::CollisionType mask);
	bool NarrowRay(ECS::Registry& registry, const Ray& ray, ContactData& data, BMask::CollisionType mask);

	Ray::Ray() {}
	Ray::Ray(BMath::Vector3 position, BMath::Vector3 direction) : origin(position), direction(direction) {}

	bool RayCast(ECS::Registry& registry, const Ray& ray, ContactData& data, BMask::CollisionType mask)
	{
		return false;
	}
	bool RayCast(ECS::Registry& registry, const Ray& ray, ContactData data[], BMask::CollisionType mask)
	{
		return false;
	}

	static bool BroadRayCheck(ECS::Registry& registry, const Ray& ray, ContactData& data, BMask::CollisionType mask)
	{
		// Static Entities
		auto staticBVHComponent = registry.Get<BVHComponent<StaticComponent>>();

		// Dynamic Entities
		auto dynamicBVHComponent = registry.Get<BVHComponent<RigidBodyComponent>>();


	}
	static bool NarrowRay(ECS::Registry& registry, const Ray& ray, ContactData& data, BMask::CollisionType mask)
	{

	}
}