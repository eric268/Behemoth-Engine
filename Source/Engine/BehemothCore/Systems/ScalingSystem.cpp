#include "pch.h"
#include "ScalingSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Misc/TransformHelper.h"

namespace Behemoth
{
	void ScalingSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto scalingComponents = registry.GetComponent<ScalingComponent>();

		if (!scalingComponents->size())
			return;

		auto components = registry.Get<ScalingComponent, TransformComponent>();
		for (const auto& [entity, scalingComp, transformComp] : components)
		{
			BMath::Vector3 oldScale = TransformHelper::ExtractScale(transformComp->localTransform);
			UpdateLocalScale(transformComp, oldScale, scalingComp->scalingVector);
			UpdateWorldScale(registry, entity, transformComp, oldScale, scalingComp->scalingVector);
			TransformHelper::NotifyChildrenTransformChange(registry, entity);
			transformComp->isDirty = true;
		}

		for (int i = scalingComponents->dense.size() - 1; i >= 0; i--)
		{
			scalingComponents->RemoveComponent(scalingComponents->dense[i]);
		}
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
			transformComp->worldTransform = TransformHelper::GetWorldTransform(registry, handle, transformComp->localTransform);
			transformComp->parentIsDirty = false;
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				transformComp->worldTransform.data[i][i] = (transformComp->worldTransform.data[i][i] / oldScale[i]) * newScale[i];
				transformComp->worldScale[i] = transformComp->worldTransform.data[i][i];
			}
		}
	}
}