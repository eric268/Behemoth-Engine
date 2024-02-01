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
		const std::string& modelFilePath,
		const std::string& texturePath,
		const std::string& entityName,
		BMath::Vector2 uvScale)
	{
		ECS::EntityHandle entityHandle = registry.CreateEntity(entityName);
		registry.AddComponent<MeshComponent>(entityHandle, modelFilePath, texturePath, uvScale);
		registry.AddComponent<MeshInitializeComponent>(entityHandle);
		registry.AddComponent<TransformComponent>(entityHandle);
		registry.AddComponent<VelocityComponent>(entityHandle);
		registry.AddComponent<RotationComponent>(entityHandle);

		return entityHandle;
	}
}