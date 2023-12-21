#include "pch.h"
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
			for (int col = 0; col < 3; col++) 
			{
				float length = sqrt(transformComp->transformMatrix.data[col][0] * transformComp->transformMatrix.data[col][0] +
										transformComp->transformMatrix.data[col][1] * transformComp->transformMatrix.data[col][1] +
										transformComp->transformMatrix.data[col][2] * transformComp->transformMatrix.data[col][2]);
				
				if (length != 0) 
				{ // Prevent division by zero
					for (int row = 0; row < 3; row++) 
					{
						transformComp->transformMatrix.data[col][row] /= length;
					}
				}
			}

			// Apply new scale
			for (int i = 0; i < 3; i++)
			{
				transformComp->transformMatrix.data[i][i] = scalingComp->scalingVector.data[i];
			}

			transformComp->scale = scalingComp->scalingVector;
		}

		for (int i = scalingComponents->dense.size() - 1; i >= 0; i--)
		{
			scalingComponents->RemoveComponent(scalingComponents->dense[i]);
		}
	}
}