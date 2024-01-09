#include "pch.h"
#include "SphereAABBCollisionSystem.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Physics/Collision/BroadCollision.h"

namespace Behemoth
{
	void SphereAABBCollisionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<TransformComponent, SphereColliderComponent>();
		auto components2 = registry.Get<TransformComponent, AABBColliderComponent>();

		for (const auto& [entity1, transformComp1, sphereComp] : components)
		{
			for (const auto& [entity2, transformComp2, aabbComp] : components2)
			{
				sphereComp->collider.worldPosition = transformComp1->worldPosition;
				aabbComp->collider.worldPosition = transformComp2->worldPosition;
				if (BroadSphereAABBCollision(sphereComp->collider, aabbComp->collider))
				{
					std::cout << "Sphere AABB colliding\n";
				}
			}
		}
	}
}