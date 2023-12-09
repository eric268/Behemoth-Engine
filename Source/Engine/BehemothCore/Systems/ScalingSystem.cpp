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
		for (const auto& [entity,scalingComp, transformComp] : components)
		{
			transformComp->transformMatrix[0][0] = scalingComp->scalingVector.x;
			transformComp->transformMatrix[1][1] = scalingComp->scalingVector.y;
			transformComp->transformMatrix[2][2] = scalingComp->scalingVector.z;
		}

		for (int i = scalingComponents->dense.size() - 1; i >= 0; i--)
		{
			scalingComponents->RemoveComponent(scalingComponents->dense[i]);
		}
	}
}