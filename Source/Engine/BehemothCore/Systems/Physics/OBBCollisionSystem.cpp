#include "pch.h"
#include "OBBCollisionSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Physics/Collision.h"

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

				OBBComp->collider.pos = transformComp->position;
				OBBComp2->collider.pos = transformComp2->position;


				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						OBBComp->collider.orientation[i][j] = transformComp->transformMatrix.data[i][j] / transformComp->scale[i];
						OBBComp2->collider.orientation[i][j] = transformComp2->transformMatrix.data[i][j] / transformComp2->scale[i];
					}
				}

				if (Collision::CheckOBBCollision(OBBComp->collider, OBBComp2->collider))
				{
					std::cout << "Colliding\n";
				}
				
			}
		}
	}
}