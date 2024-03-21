#include "pch.h"
#include "MovementSystem.h"
#include "Components/Components.h"
#include "Misc/TransformHelper.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Core/Log.h"

namespace Behemoth
{
	void MovementSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
 		auto components = registry.Get<TransformComponent, MoveComponent>();

		// Need to sort the move components in order before applying move. Parent entities must be moved before their children.
		// so that the child can properly calculate its world position
		std::sort(components.begin(), components.end(),
			[&](std::tuple<ECS::Entity, TransformComponent*, MoveComponent*> tuple1, std::tuple<ECS::Entity, TransformComponent*, MoveComponent*> tuple2)
			{
				return std::get<0>(tuple1).GetIdentifier() < std::get<0>(tuple2).GetIdentifier();

			});

		for (const auto& [entityHandle, transformComp, movementComp] : components)
		{
			// No need to move entity if delta movement is 0 and is not additive
			if (movementComp->location == BMath::Vector3::Zero() && movementComp->isAdditive)
			{
				registry.RemoveComponent<MoveComponent>(entityHandle);
				continue;
			}

			BMath::Vector3 deltaLocation = movementComp->location;
			TransformComponent* parentTransformComp = TransformHelper::GetParentTransformComp(registry, entityHandle);

			if (movementComp->isAdditive)
			{
				if (parentTransformComp)
				{
					// If entity is a child, move in parents local space
					BMath::Matrix3x3 parentTransformNoScale = TransformHelper::ExtractRotationMatrix(
						parentTransformComp->worldTransform,
						parentTransformComp->worldScale);

					deltaLocation = parentTransformNoScale * deltaLocation;
				}

				UpdateLocalTransform(registry, entityHandle, transformComp, deltaLocation);
			}
			else
			{
				// If entity is a child need to move relative to parents position
				if (parentTransformComp)
				{
					deltaLocation = movementComp->location - parentTransformComp->worldPosition;
				}

				transformComp->localPosition = deltaLocation;
				transformComp->localTransform._41 = transformComp->localPosition.x;
				transformComp->localTransform._42 = transformComp->localPosition.y;
				transformComp->localTransform._43 = transformComp->localPosition.z;
			}

			TransformHelper::UpdateWorldTransform(registry, entityHandle, transformComp);
			TransformHelper::NotifyChildrenTransformChange(registry, entityHandle);
			transformComp->isDirty = true;

			if (CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(entityHandle))
			{
				cameraComponent->isDirty = true;
			}

			registry.RemoveComponent<MoveComponent>(entityHandle);
		}
	}

	void MovementSystem::UpdateLocalTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp, BMath::Vector3 deltaLocation)
	{
		transformComp->localTransform._41 += deltaLocation.x;
		transformComp->localTransform._42 += deltaLocation.y;
		transformComp->localTransform._43 += deltaLocation.z;
		transformComp->localPosition      += deltaLocation;
	}
}
