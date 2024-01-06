#include "pch.h"
#include "AABBCollisionSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Physics/Collision.h"


namespace Behemoth
{
	void AABBCollisionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<TransformComponent, AABBColliderComponent>();
		for (const auto& [entity, transformComp, AABBComp] : components)
		{
			for (int i = 0; i < components.size(); i++)
			{
				auto& [entity2, transformComp2, AABBComp2] = components[i];
				if (entity.GetIdentifier() == entity2.GetIdentifier())
					continue;

				AABBComp->collider.position = transformComp->position;
				AABBComp2->collider.position = transformComp2->position;

				if (Collision::CheckAABBCollision(AABBComp->collider, AABBComp->collider))
				{
					// std::cout << "Colliding\n";
				}
			}
		}
	}
}