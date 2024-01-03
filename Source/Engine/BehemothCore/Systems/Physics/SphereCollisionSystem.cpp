#include "pch.h"
#include "SphereCollisionSystem.h"
#include "Components/Components.h"
#include "Physics/Collision.h"

namespace Behemoth
{
	void SphereCollisionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<SphereColliderComponent, TransformComponent>();

		for (const auto& [entity, sphereColliderComp, transformComp] : components)
		{
			for (const auto& [entity2, sphereColliderComp2, transformComp2] : components)
			{
				if (entity.GetIdentifier() == entity2.GetIdentifier())
				{
					continue;
				}

				if (CheckSphereSphereCollision(transformComp->position, sphereColliderComp->radius, transformComp2->position, sphereColliderComp2->radius))
				{
					std::cout << "Sphere Sphere Collision\n";
				}
			}
		}
	}
}