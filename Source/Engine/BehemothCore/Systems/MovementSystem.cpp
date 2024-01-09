#include "pch.h"
#include "MovementSystem.h"
#include "Components/Components.h"
#include "Misc/TransformHelper.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Misc/Log.h"

namespace Behemoth
{
	void MovementSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto movementComponents = registry.GetComponent<MoveComponent>();

		if (!movementComponents->size())
			return;

		auto components = registry.Get<TransformComponent, MoveComponent>();
		for (const auto& [entity, transformComp, movementComp] : components)
		{
			UpdateLocalTransform(transformComp, movementComp);
			UpdateWorldTransform(registry, entity, transformComp, movementComp);
			TransformHelper::NotifyChildrenTransformChange(registry, entity);

			CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(entity);
			if (cameraComponent && cameraComponent->isMain)
			{
				cameraComponent->isDirty = true;
			}
		}

		for (int i = movementComponents->dense.size() - 1; i >= 0; i--)
		{
			movementComponents->RemoveComponent(movementComponents->dense[i]);
		}
	}

	void MovementSystem::UpdateLocalTransform(TransformComponent* transformComp, MoveComponent* movementComp)
	{
		transformComp->isDirty = true;
		transformComp->localTransform._41 = movementComp->location.x;
		transformComp->localTransform._42 = movementComp->location.y;
		transformComp->localTransform._43 = movementComp->location.z;
		transformComp->localPosition = movementComp->location;
	}

	void MovementSystem::UpdateWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp, MoveComponent* movementComp)
	{
		if (transformComp->parentIsDirty)
		{
			transformComp->worldTransform = TransformHelper::GetWorldTransform(registry, handle, transformComp->localTransform);
			transformComp->parentIsDirty = false;
		}
		else
		{
			transformComp->worldTransform._41 += movementComp->location.x;
			transformComp->worldTransform._42 += movementComp->location.y;
			transformComp->worldTransform._43 += movementComp->location.z;
		}

		transformComp->worldPosition = BMath::Vector3(transformComp->worldTransform._41, transformComp->worldTransform._42, transformComp->worldTransform._43);
	}
}