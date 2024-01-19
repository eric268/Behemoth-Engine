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
		registry.AddComponent<RotationComponent>(entity);
		registry.AddComponent<TransformComponent>(entity);
		registry.AddComponent<MoveComponent>(entity, BMath::Vector3(0.0f, 0.0f, 0.0f));
		
		return entity;
	}
}