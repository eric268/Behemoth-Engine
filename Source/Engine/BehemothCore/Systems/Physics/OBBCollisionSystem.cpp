#include "pch.h"
#include "OBBCollisionSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Physics/Collision/BroadCollision.h"

namespace Behemoth
{
	void OBBCollisionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<OBBColliderComponent, TransformComponent>();

		for (const auto& [entity, OBBComp, transformComp] : components)
		{
			for (const auto& [entity2, OBBComp2, transformComp2] : components)
			{
				if (entity == entity2)
				{
					continue;
				}

				// OBBComp->collider.worldTransform = transformComp->worldTransform * OBBComp->localTransformOffset;
				// OBBComp2->collider.worldTransform = transformComp2->worldTransform * OBBComp2->localTransformOffset;

				if (BroadOBBCollision(OBBComp->collider, OBBComp2->collider))
				{
					std::cout << "Colliding\n";
				}
				
			}
		}
	}
}