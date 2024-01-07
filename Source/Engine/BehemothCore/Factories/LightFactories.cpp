#include "pch.h"
#include "LightFactories.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Components/LightComponents.h"

namespace Behemoth
{
	ECS::EntityHandle DirectionalLightFactory::CreateDirectionalLight(ECS::Registry& registry, const std::string& name)
	{
		ECS::EntityHandle entity = registry.CreateEntity(name);
		registry.AddComponent<DirectionalLightComponent>(entity);
		return entity;
	}

	ECS::EntityHandle PointLightFactory::CreatePointLight(ECS::Registry& registry, const std::string& name)
	{
		ECS::EntityHandle entity = registry.CreateEntity(name);

		registry.AddComponent<PointLightComponent>(entity, BMath::Vector3(0.75f, 0.75f, 0.75f), 1.0f);
		registry.AddComponent<TransformComponent>(entity);
		registry.AddComponent<MoveComponent>(entity, BMath::Vector3(0.0f, 0.0f, 0.0f));
#ifdef DEBUG
		registry.AddComponent<MeshComponent>(entity, "cube.obj", "brick.png", BMath::Vector2(0.5f, 0.5f), true);
		registry.AddComponent<ScalingComponent>(entity, BMath::Vector3(0.3f, 0.3f, 0.3f));
#endif
		return entity;
	}
}