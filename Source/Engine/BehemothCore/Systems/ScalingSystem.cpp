#include "ScalingSystem.h"
#include "Components/Components.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	void ScalingSystem::Run(ECS::Registry& registry)
	{
		auto scalingComponents = registry.GetComponent<ScalingComponent>();

		if (!scalingComponents->size())
			return;

		auto components = registry.Get<ScalingComponent, TransformComponent>();
		for (const auto& [entity, scalingComp, transformComp] : components)
		{
			for (int i = 0; i < 3; ++i) 
			{
				float length = sqrt(transformComp->transformMatrix[i][0] * transformComp->transformMatrix[i][0] +
					transformComp->transformMatrix[i][1] * transformComp->transformMatrix[i][1] +
					transformComp->transformMatrix[i][2] * transformComp->transformMatrix[i][2]);
				
				if (length != 0) 
				{ // Prevent division by zero
					for (int j = 0; j < 3; ++j) 
					{
						transformComp->transformMatrix[i][j] /= length;
					}
				}
			}

			// Apply new scale
			transformComp->transformMatrix[0][0] = scalingComp->scalingVector.x;
			transformComp->transformMatrix[1][1] = scalingComp->scalingVector.y;
			transformComp->transformMatrix[2][2] = scalingComp->scalingVector.z;

			transformComp->scale = scalingComp->scalingVector;
		}

		for (int i = scalingComponents->dense.size() - 1; i >= 0; i--)
		{
			scalingComponents->RemoveComponent(scalingComponents->dense[i]);
		}
	}
}