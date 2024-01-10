#include "pch.h"
#include "AABBCollisionSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Physics/Collision/BroadCollision.h"


namespace Behemoth
{
	void AABBCollisionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<TransformComponent, AABBColliderComponent>();
		for (const auto& [entity, transformComp, AABBComp] : components)
		{
			for (const auto& [entity2, transformComp2, AABBComp2] : components)
			{
				if (entity.GetIdentifier() == entity2.GetIdentifier())
				{
					continue;
				}

				AABBComp->collider.worldPosition = transformComp->worldPosition;
				AABBComp2->collider.worldPosition = transformComp2->worldPosition;

				AABBComp->collider.worldExtents = transformComp->worldScale * AABBComp->extents;
				AABBComp2->collider.worldExtents = transformComp2->worldScale * AABBComp2->extents;

				if (BroadAABBCollision(AABBComp->collider, AABBComp2->collider))
				{
					std::cout << "Colliding\n";
				}
			}
		}
	}
}