#include "pch.h"
#include "CameraFactory.h"
#include "Components/Components.h"

namespace Behemoth
{
	ECS::Entity CameraFactory::CreateCamera(ECS::Registry& registry, bool isMain, const std::string& name)
	{
		ECS::Entity entity = registry.CreateEntity(name);
		registry.AddComponent<CameraComponent>(entity, isMain);
		registry.AddComponent<VelocityComponent>(entity);
		registry.AddComponent<RotationComponent>(entity);
		registry.AddComponent<TransformComponent>(entity);
		registry.AddComponent<MoveComponent>(entity, Math::Vector3(0.0f, 0.0f, 0.0f));
		
		return entity;
	}
}