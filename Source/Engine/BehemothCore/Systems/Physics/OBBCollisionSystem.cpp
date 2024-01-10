#include "pch.h"
#include "OBBCollisionSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Physics/Collision/NarrowCollision.h"

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

				OBBComp->collider.worldPosition = transformComp->worldPosition;
				OBBComp2->collider.worldPosition = transformComp2->worldPosition;

				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						OBBComp->collider.worldOrientation[i][j] = transformComp->worldTransform.data[i][j] / transformComp->worldScale[i];
						OBBComp2->collider.worldOrientation[i][j] = transformComp2->worldTransform.data[i][j] / transformComp2->worldScale[i];
					}
				}

// 				if (BroadOBBCollision(OBBComp->collider, OBBComp2->collider))
// 				{
// 					std::cout << "Colliding\n";
// 				}
			}
		}
	}
}