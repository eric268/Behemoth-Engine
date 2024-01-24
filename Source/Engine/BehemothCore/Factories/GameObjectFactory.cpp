#include "pch.h"
#include "GameObjectFactory.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Components/PhysicsComponents.h"

namespace Behemoth
{
	ECS::EntityHandle GameObjectFactory::CreateGameObject(
		ECS::Registry& registry,
		std::string modelFilePath,
		std::string texturePath,
		std::string entityName,
		BMath::Vector2 uvScale)
	{
		ECS::EntityHandle entity = registry.CreateEntity(entityName);
		registry.AddComponent<MeshComponent>(entity, modelFilePath, texturePath, uvScale);
		registry.AddComponent<MeshInitalizeComponent>(entity);
		registry.AddComponent<TransformComponent>(entity);
		registry.AddComponent<VelocityComponent>(entity);
		registry.AddComponent<RotationComponent>(entity);

		return entity;
	}

	bool GameObjectFactory::AddChildObject(
		ECS::Registry& registry,
		const ECS::EntityHandle& parentHandle,
		const ECS::EntityHandle& childHandle)
	{
		if (!registry.IsValidHandle(parentHandle) || !registry.IsValidHandle(childHandle))
		{
			LOGMESSAGE(MessageType::Error, "Invalid parent handle found");
			return false;
		}

		// If parent doesn't already have a parent component, then add it
		ParentComponent* parentComp = registry.GetComponent<ParentComponent>(parentHandle);
		if (!parentComp)
		{
			registry.AddComponent<ParentComponent>(parentHandle);
			// Ensure that component has been added successfully
			parentComp = registry.GetComponent<ParentComponent>(parentHandle);
			if (!parentComp)
			{
				LOGMESSAGE(MessageType::Error, "Error adding parent component to entity: " + registry.GetName(parentHandle));
				return false;
			}
		}

		if (TransformComponent* transformComponent = registry.GetComponent<TransformComponent>(childHandle))
		{
			transformComponent->parentIsDirty = true;
		}

		registry.AddComponent<ChildComponent>(childHandle, parentHandle);
		parentComp->childHandles.push_back(childHandle);
		return true;
	}
}