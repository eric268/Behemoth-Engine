#include "pch.h"
#include "GameObjectFactory.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"

namespace Behemoth
{
	ECS::EntityHandle GameObjectFactory::CreateGameObject(ECS::Registry& registry, std::string modelFilePath, std::string texturePath, std::string entityName)
	{
		ECS::EntityHandle entity = registry.CreateEntity(entityName);
		registry.AddComponent<MeshComponent>(entity, modelFilePath, texturePath);
		registry.AddComponent<TransformComponent>(entity);
		registry.AddComponent<MeshInitalizeComponent>(entity);
		registry.AddComponent<VelocityComponent>(entity);
		registry.AddComponent<RotationComponent>(entity, 0.0f, 0.0f);
		registry.AddComponent<BoundingVolumeComponent>(entity, 1.5f, false);

		return entity;
	}
}