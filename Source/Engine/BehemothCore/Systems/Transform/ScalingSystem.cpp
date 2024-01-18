#include "pch.h"
#include "ScalingSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Misc/TransformHelper.h"

#include "Application/ThreadPool.h"

namespace Behemoth
{
	void ScalingSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<ScalingComponent, TransformComponent>();

		// Iterate over the container backwards because we want to remove all of these components once the scaling is completed
		for (const auto& [entity, scalingComp, transformComp] : components)
		{
			ScaleEntities(registry, scalingComp, transformComp, entity);
			// ThreadPool::GetInstance().Enqueue(&ScalingSystem::ScaleEntities, this, std::ref(registry), scalingComp, transformComp, entity);
		}
		ThreadPool::GetInstance().WaitForCompletion();
	}

	void ScalingSystem::ScaleEntities(ECS::Registry& registry, ScalingComponent* scalingComp, TransformComponent* transformComp, const ECS::EntityHandle& handle)
	{
		BMath::Vector3 oldScale = TransformHelper::ExtractScale(transformComp->localTransform);
		UpdateLocalScale(transformComp, oldScale, scalingComp->scalingVector);
		UpdateWorldScale(registry, handle, transformComp, oldScale, scalingComp->scalingVector);
		TransformHelper::NotifyChildrenTransformChange(registry, handle);
		transformComp->isDirty = true;

		registry.RemoveComponent<ScalingComponent>(handle);
	}

	void ScalingSystem::UpdateLocalScale(TransformComponent* transformComp, const BMath::Vector3& oldScale, const BMath::Vector3& newScale)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				transformComp->localTransform.data[i][j] /= oldScale[i];
			}
		}
		// Apply new scale
		for (int i = 0; i < 3; i++)
		{
			transformComp->localTransform.data[i][i] = newScale[i];
		}

		transformComp->localScale = newScale;
	}

	void ScalingSystem::UpdateWorldScale(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp, const BMath::Vector3& oldScale, const BMath::Vector3& newScale)
	{
		if (transformComp->parentIsDirty)
		{
			transformComp->worldScale = TransformHelper::GetParentScale(registry, handle) * transformComp->localScale;

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					transformComp->worldTransform.data[i][j] /= transformComp->worldScale[i];
				}
			}
			// Apply new scale
			for (int i = 0; i < 3; i++)
			{
				transformComp->worldTransform.data[i][i] = transformComp->worldScale[i];
			}

			transformComp->parentIsDirty = false;

		}
		else
		{

			transformComp->worldTransform = transformComp->localTransform;
			transformComp->worldScale = transformComp->localScale;
		}
	}
}