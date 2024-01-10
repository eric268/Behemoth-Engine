#include "pch.h"
#include "SphereCollisionSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Physics/Collision/BroadCollision.h"

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

				sphereColliderComp->collider.position = transformComp->worldPosition;
				sphereColliderComp2->collider.position = transformComp2->worldPosition;


				if (BroadSphereCollision(sphereColliderComp->collider, sphereColliderComp->collider))
				{
					std::cout << "Sphere Sphere Collision\n";
				}
			}
		}
	}
}