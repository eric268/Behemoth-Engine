#include "pch.h"
#include "GameObjectHelper.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/Components.h"

namespace Behemoth
{
	bool GameObjectHelper::AddChildObject(
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

		if (TransformComponent* transformComp = registry.GetComponent<TransformComponent>(childHandle))
		{
			transformComp->parentIsDirty = true;
		}

		registry.AddComponent<ChildComponent>(childHandle, parentHandle);
		parentComp->childHandles.push_back(childHandle);
		return true;
	}
}