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
				LOGMESSAGE(MessageType::Warning, "Attempting to move static entity named: " + registry.GetName(entity));
				continue;
			}

			if (BMath::Vector3::Equals(velocityComp->velocity, BMath::Vector3::Zero()) && !transformComp->parentIsDirty)
			{
				continue;
			}

			BMath::Vector3 deltaPosition = velocityComp->velocity * deltaTime;

			TransformComponent* parentTransform = TransformHelper::GetParentTransformComp(registry, entity);
			if (parentTransform)
			{
				// If object is a child, move in parents local space
				BMath::Matrix3x3f parentTransformNoScale = TransformHelper::ExtractRotationMatrix(parentTransform->worldTransform, parentTransform->worldScale);

				deltaPosition = parentTransformNoScale * deltaPosition;
			}

			UpdateLocalTransform(registry, entity, transformComp, deltaPosition);
			TransformHelper::UpdateWorldTransform(registry, entity, transformComp);
			TransformHelper::NotifyChildrenTransformChange(registry, entity);

			CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(entity);
			if (cameraComponent && cameraComponent->isMain)
			{
				cameraComponent->isDirty = true;
			}

			TransformHelper::NotifyChildrenTransformChange(registry, entity);
		}
	}

	void VelocitySystem::UpdateLocalTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp, BMath::Vector3 deltaPosition)
	{
		transformComp->localTransform._41 += deltaPosition.x;
		transformComp->localTransform._42 += deltaPosition.y;
		transformComp->localTransform._43 += deltaPosition.z;
		transformComp->localPosition += deltaPosition;
		transformComp->isDirty = true;
	}
}
