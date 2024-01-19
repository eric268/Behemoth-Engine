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

		for (const auto& [entity, transformComp, movementComp] : components)
		{
			if (movementComp->location == BMath::Vector3::Zero())
			{
				continue;
			}

			BMath::Vector3 deltaLocation = movementComp->location;

			TransformComponent* parentTransform = TransformHelper::GetParentTransformComp(registry, entity);
			if (parentTransform)
			{
				// If object is a child, move in parents local space
				BMath::BMatrix3x3 parentTransformNoScale = TransformHelper::ExtractRotationMatrix(parentTransform->worldTransform, parentTransform->worldScale);

				deltaLocation = parentTransformNoScale * deltaLocation;
			}

			UpdateLocalTransform(registry, entity, transformComp, deltaLocation);
			TransformHelper::UpdateWorldTransform(registry, entity, transformComp);
			transformComp->worldPosition = BMath::Vector3(transformComp->worldTransform._41, transformComp->worldTransform._42, transformComp->worldTransform._43);

			TransformHelper::NotifyChildrenTransformChange(registry, entity);
			MarkViewDirtyIfCamera(registry, entity);
			registry.RemoveComponent<MoveComponent>(entity);
		}
	}

	void MovementSystem::UpdateLocalTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp, BMath::Vector3 deltaLocation)
	{
		transformComp->localTransform._41 += deltaLocation.x;
		transformComp->localTransform._42 += deltaLocation.y;
		transformComp->localTransform._43 += deltaLocation.z;
		transformComp->localPosition += deltaLocation;
		transformComp->isDirty = true;
	}


	void MovementSystem::MarkViewDirtyIfCamera(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
	{
		CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(entityHandle);
		if (cameraComponent && cameraComponent->isMain)
		{
			cameraComponent->isDirty = true;
		}
	}
}
