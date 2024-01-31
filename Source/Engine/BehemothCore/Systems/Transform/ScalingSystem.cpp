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
		for (const auto& [
			entity, 
				scalingComp,
				transformComp] : registry.Get<ScalingComponent, TransformComponent>())
		{
			ScaleEntities(registry, scalingComp, transformComp, entity);
			transformComp->isDirty = true;
			registry.RemoveComponent<ScalingComponent>(entity);
		}
	}

	void ScalingSystem::ScaleEntities(ECS::Registry& registry, ScalingComponent* scalingComp, TransformComponent* transformComp, const ECS::EntityHandle& handle)
	{
		UpdateLocalScale(transformComp, transformComp->localScale, scalingComp->scalingVector);
		TransformHelper::UpdateWorldTransform(registry, handle, transformComp);
		TransformHelper::NotifyChildrenTransformChange(registry, handle);
	}

	void ScalingSystem::UpdateLocalScale(TransformComponent* transformComp, const BMath::Vector3& oldScale, const BMath::Vector3& newScale)
	{
		BMath::Matrix4x4 m = TransformHelper::RemoveScale(transformComp->localTransform);
		BMath::Matrix4x4 scaleMatrix = BMath::Matrix4x4::Identity();

		for (int i = 0; i < 3; i++)
		{
			scaleMatrix.data[i][i] = newScale[i];
		}

		transformComp->localTransform = m * scaleMatrix;
		transformComp->localScale = newScale;
	}
}