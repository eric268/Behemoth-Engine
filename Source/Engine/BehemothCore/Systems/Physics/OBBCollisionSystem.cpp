#include "pch.h"
#include "OBBCollisionSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
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

				OBB box1;
				box1.point = transformComp->position;
				box1.halfwidthExtents = OBBComp->maxPoint;

				OBB box2;
				box2.point = transformComp2->position;
				box2.halfwidthExtents = OBBComp2->maxPoint;

				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						box1.rotVecs[i][j] = transformComp->transformMatrix.data[i][j] / transformComp->scale[i];
						box2.rotVecs[i][j] = transformComp2->transformMatrix.data[i][j] / transformComp2->scale[i];
					}
				}

				if (CheckOBBCollision(box1, box2))
				{
					std::cout << "Colliding\n";
				}
				
			}
		}
	}
}