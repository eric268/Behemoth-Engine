#include "pch.h"
#include "ScalingSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Misc/TransformHelper.h"

#include "Core/ThreadPool.h"

namespace Behemoth
{
	void ScalingSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<ScalingComponent, TransformComponent>();

		// Iterate over the container backwards because we want to remove all of these components once the scaling is completed
		for (const auto& [entity, scalingComp, transformComp] : components)
		{
// 			if (BMath::Vector3::Equals(scalingComp->scalingVector, transformComp->localScale) && !transformComp->parentIsDirty)
// 			{
// 				continue;
// 			}

			ScaleEntities(registry, scalingComp, transformComp, entity);
			registry.RemoveComponent<ScalingComponent>(entity);
		}
	}

	void ScalingSystem::ScaleEntities(ECS::Registry& registry, ScalingComponent* scalingComp, TransformComponent* transformComp, const ECS::EntityHandle& handle)
	{
		UpdateLocalScale(transformComp, transformComp->localScale, scalingComp->scalingVector);

		TransformHelper::UpdateWorldTransform(registry, handle, transformComp);
		TransformHelper::NotifyChildrenTransformChange(registry, handle);
		transformComp->isDirty = true;
	}

	void ScalingSystem::UpdateLocalScale(TransformComponent* transformComp, const BMath::Vector3& oldScale, const BMath::Vector3& newScale)
	{
		BMath::Matrix4x4f m = TransformHelper::RemoveScale(transformComp->localTransform, oldScale);

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				m.data[i][j] *= newScale[i];
			}
		}

		transformComp->localTransform = m;
		transformComp->localScale = newScale;

	}
}