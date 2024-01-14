#include "pch.h"
#include "CollisionResolutionSystem.h"
#include "ECS/Registry.h"
#include "Physics/Collision/Colliders.h"
#include "Components/PhysicsComponents.h"
#include "Components/Components.h"

namespace Behemoth
{
	void CollisionResolutionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [entity, transformComp, collisionData] : registry.Get<TransformComponent, CollisionDataComponent>())
		{
			LOGMESSAGE(General, "Working");
		}


		auto componentsToRemove = registry.GetComponent<CollisionDataComponent>();
		if (componentsToRemove)
		{
			for (int i = componentsToRemove->dense.size() - 1; i >= 0; i--)
			{
				componentsToRemove->RemoveComponent(componentsToRemove->dense[i]);
			}
		}
	}
}