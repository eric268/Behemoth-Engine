#include "pch.h"
#include "CameraFactory.h"
#include "Components/Components.h"

namespace Behemoth
{
	ECS::Entity CameraFactory::CreateCamera(ECS::Registry& registry, bool isMain, const std::string& name)
	{
		ECS::Entity entity = registry.CreateEntity(name);
		registry.AddComponent<Behemoth::CameraComponent>(entity, isMain);
		registry.AddComponent<Behemoth::VelocityComponent>(entity);
		registry.AddComponent<Behemoth::RotationComponent>(entity);
		registry.AddComponent<Behemoth::TransformComponent>(entity);
		registry.AddComponent<Behemoth::MoveComponent>(entity, Math::Vector3(0.0f, 0.0f, 0.0f));
		
		return entity;
	}
}