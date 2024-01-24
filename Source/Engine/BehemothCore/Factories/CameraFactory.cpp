#include "pch.h"
#include "CameraFactory.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/Components.h"

namespace Behemoth
{
	ECS::EntityHandle CameraFactory::CreateCamera(ECS::Registry& registry, bool isMain, const std::string& name)
	{
		ECS::EntityHandle entity = registry.CreateEntity(name);
		registry.AddComponent<CameraComponent>(entity, isMain);
		registry.AddComponent<VelocityComponent>(entity);
		registry.AddComponent<TransformComponent>(entity);
		return entity;
	}
}