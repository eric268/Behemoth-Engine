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

		auto components = registry.Get<TransformComponent, MoveComponent>();

		// Need to sort the move components in order before applying move. Parent entities must be created before their children, but move components can be added after the child has been created.
		// parents move component must be applied first so that the child can properly calculate its world position. Entities created before 
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

			if (movementComp->localMove)
			{
				UpdateLocalTransform(transformComp, movementComp);
				UpdateWorldTransform(registry, entity, transformComp, movementComp);
			}
			// Work on this later, issue is that 
// 			else
// 			{
// 				BMath::Vector3 parentScale = BMath::Vector3::One();
// 				if (ChildComponent* childComp = registry.GetComponent<ChildComponent>(entity))
// 				{
// 					if (TransformComponent* parentTransformComp = registry.GetComponent<TransformComponent>(childComp->parentHandle))
// 					{
// 						BMath::Vector3 parentWorldPos = parentTransformComp->worldPosition;
// 						parentScale = parentTransformComp->worldScale;
// 						transformComp->localPosition = movementComp->location - parentWorldPos;
// 					}
// 					else
// 					{
// 						LOG_MESSAGE(MessageType::Error, "Error getting parent components transform");
// 					}
// 				}
// 				else
// 				{
// 					transformComp->localPosition = movementComp->location;
// 				}
// 
// 				transformComp->localTransform._41 = transformComp->localPosition.x;
// 				transformComp->localTransform._42 = transformComp->localPosition.y;
// 				transformComp->localTransform._43 = transformComp->localPosition.z;
// 				
// 				if (transformComp->parentIsDirty)
// 				{
// 					transformComp->worldTransform = TransformHelper::GetWorldTransform(registry, entity, transformComp->localTransform);					
// 					transformComp->parentIsDirty = false;
// 				}
// 			}

			transformComp->isDirty = true;
			transformComp->worldPosition = BMath::Vector3(transformComp->worldTransform._41, transformComp->worldTransform._42, transformComp->worldTransform._43);
			TransformHelper::NotifyChildrenTransformChange(registry, entity);

			CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(entity);
			if (cameraComponent && cameraComponent->isMain)
			{
				cameraComponent->isDirty = true;
			}

			registry.RemoveComponent<MoveComponent>(entity);
		}
	}

	void MovementSystem::UpdateLocalTransform(TransformComponent* transformComp, MoveComponent* movementComp)
	{
		transformComp->localTransform._41 += movementComp->location.x;
		transformComp->localTransform._42 += movementComp->location.y;
		transformComp->localTransform._43 += movementComp->location.z;
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
	}
}
