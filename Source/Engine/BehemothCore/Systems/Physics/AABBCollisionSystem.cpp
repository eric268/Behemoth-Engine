#include "pch.h"
#include "AABBCollisionSystem.h"
#include "Components/Components.h"
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

				Math::Vector3 box1Min = transformComp->position + AABBComp->minPoint;
				Math::Vector3 box1Max = transformComp->position + AABBComp->maxPoint;

				Math::Vector3 box2Min = transformComp2->position + AABBComp2->minPoint;
				Math::Vector3 box2Max = transformComp2->position + AABBComp2->maxPoint;

				if (CheckAABBCollision(box1Min, box1Max, box2Min, box2Max))
				{
					std::cout << "Colliding\n";
				}
				else
					std::cout << "NOT Colliding\n";
			}
		}
	}
}