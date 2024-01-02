#include "pch.h"
#include "GameObjectFactory.h"
#include "Components/Components.h"

namespace Behemoth
{
	ECS::Entity GameObjectFactory::CreateGameObject(ECS::Registry& registry, std::string& modelFilePath, std::string texturePath, std::string entityName)
	{
		ECS::Entity entity = registry.CreateEntity(entityName);
		registry.AddComponent<Behemoth::MeshComponent>(entity, modelFilePath, texturePath);
		registry.AddComponent<Behemoth::TransformComponent>(entity);
		registry.AddComponent<Behemoth::MeshInitalizeComponent>(entity);
		registry.AddComponent<Behemoth::VelocityComponent>(entity);
		registry.AddComponent<Behemoth::RotationComponent>(entity, 0.0f, 0.0f);
		registry.AddComponent<Behemoth::ScalingComponent>(entity, Math::Vector3(1.0f, 1.0f, 1.0f));
		registry.AddComponent<Behemoth::BoundingVolumeComponent>(entity, 1.5f, false);

		return entity;
	}
}