#include "pch.h"
#include "VelocitySystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Misc/TransformHelper.h"
#include "Core/Log.h"

namespace Behemoth
{
	void VelocitySystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [
			entityHandle,
				velocityComp,
				transformComp] : registry.Get<VelocityComponent, TransformComponent>())
		{
			if (registry.GetComponent<StaticComponent>(entityHandle) && velocityComp->velocity != BMath::Vector3::Zero())
			{
				velocityComp->velocity = BMath::Vector3::Zero();
				LOGMESSAGE(MessageType::Warning, "Attempting to move static entity named: " + registry.GetName(entityHandle));
				continue;
			}

			if (BMath::Vector3::Equals(velocityComp->velocity, BMath::Vector3::Zero()) && !transformComp->parentIsDirty)
			{
				continue;
			}

			BMath::Vector3 deltaPosition = velocityComp->velocity * deltaTime;

			if (TransformComponent* parentTransformComp = TransformHelper::GetParentTransformComp(registry, entityHandle))
			{
				// If object is a child, move in parents local space
				BMath::Matrix3x3 parentTransformNoScale = TransformHelper::ExtractRotationMatrix(
					parentTransformComp->worldTransform,
							     parentTransformComp->worldScale);

				deltaPosition = parentTransformNoScale * deltaPosition;
			}

			UpdateLocalTransform(registry, entityHandle, transformComp, deltaPosition);
			TransformHelper::UpdateWorldTransform(registry, entityHandle, transformComp);
			TransformHelper::NotifyChildrenTransformChange(registry, entityHandle);
			transformComp->isDirty = true;

			if (CameraComponent* cameraComp = registry.GetComponent<CameraComponent>(entityHandle))
			{
				cameraComp->isDirty = true;
			}
		}
	}

	void VelocitySystem::UpdateLocalTransform(
		ECS::Registry& registry, 
		const ECS::EntityHandle& handle,
		TransformComponent* transformComp,
		const BMath::Vector3& deltaPosition)
	{
		transformComp->localTransform._41 += deltaPosition.x;
		transformComp->localTransform._42 += deltaPosition.y;
		transformComp->localTransform._43 += deltaPosition.z;
		transformComp->localPosition += deltaPosition;
	}
}
