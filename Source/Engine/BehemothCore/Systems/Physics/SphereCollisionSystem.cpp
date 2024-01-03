#include "pch.h"
#include "SphereCollisionSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
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
				if (entity == entity2)
				{
					continue;
				}

				sphereColliderComp->collider.pos = transformComp->position;
				sphereColliderComp2->collider.pos = transformComp2->position;


				if (CheckCollision(sphereColliderComp->collider, sphereColliderComp->collider))
				{
					std::cout << "Sphere Sphere Collision\n";
				}
			}
		}
	}
}