#include "pch.h"
#include "VelocitySystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Misc/TransformHelper.h"
#include "Misc/Log.h"

namespace Behemoth
{
	void VelocitySystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<VelocityComponent, TransformComponent>();
		for (const auto& [entity, velocityComp, transformComp] : components)
		{
			if (registry.GetComponent<StaticComponent>(entity) && velocityComp->velocity != BMath::Vector3::Zero())
			{
				velocityComp->velocity = BMath::Vector3::Zero();
				LOG_MESSAGE(MessageType::Warning, "Attempting to move static entity named: " + registry.GetName(entity));
				continue;
			}

			if (BMath::Vector3::Equals(velocityComp->velocity, BMath::Vector3::Zero()) && !transformComp->parentIsDirty)
			{
				continue;
			}

			BMath::Vector3 deltaPosition = velocityComp->velocity * deltaTime;
			UpdateLocalTransform(transformComp, deltaPosition);
			UpdateWorldTransform(registry, entity, transformComp, deltaPosition);
			TransformHelper::NotifyChildrenTransformChange(registry, entity);

			CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(entity);
			if (cameraComponent && cameraComponent->isMain)
			{
				cameraComponent->isDirty = true;
			}

			TransformHelper::NotifyChildrenTransformChange(registry, entity);
		}
	}

	void VelocitySystem::UpdateLocalTransform(TransformComponent* transformComp, const BMath::Vector3& deltaPosition)
	{
		transformComp->localTransform._41 += deltaPosition.x;
		transformComp->localTransform._42 += deltaPosition.y;
		transformComp->localTransform._43 += deltaPosition.z;
		transformComp->localPosition += deltaPosition;
		transformComp->isDirty = true;
	}

	void VelocitySystem::UpdateWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp, const BMath::Vector3& deltaPosition)
	{
		if (transformComp->parentIsDirty)
		{
			transformComp->worldTransform = TransformHelper::GetWorldTransform(registry, handle, transformComp->localTransform);
			transformComp->parentIsDirty = false;
		}
		else
		{
			transformComp->worldTransform._41 += deltaPosition.x;
			transformComp->worldTransform._42 += deltaPosition.y;
			transformComp->worldTransform._43 += deltaPosition.z;
			transformComp->worldPosition += deltaPosition;
			transformComp->isDirty = true;
		}
	}
}
