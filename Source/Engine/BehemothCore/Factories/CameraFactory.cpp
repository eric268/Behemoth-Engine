#include "pch.h"
#include "CameraFactory.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/Components.h"

namespace Behemoth
{
	ECS::EntityHandle CameraFactory::CreateCamera(ECS::Registry& registry, bool isMain, const std::string& name)
	{
		ECS::EntityHandle entityHandle = registry.CreateEntity(name);
		registry.AddComponent<CameraComponent>(entityHandle, isMain);
		registry.AddComponent<VelocityComponent>(entityHandle);
		registry.AddComponent<TransformComponent>(entityHandle);
		return entityHandle;
	}
}