#include "pch.h"
#include "LightFactories.h"
#include "Components/Components.h"

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

		registry.AddComponent<PointLightComponent>(entity, Math::Vector3(0.75f, 0.75f, 0.75f), 1.0f);
		registry.AddComponent<TransformComponent>(entity);
		registry.AddComponent<MoveComponent>(entity, Math::Vector3(0.0f, 0.0f, 0.0f));
#ifdef DEBUG
		registry.AddComponent<MeshComponent>(entity, "cube.obj", "brick.png", Math::Vector2(0.5f, 0.5f), false);
		registry.AddComponent<ScalingComponent>(entity, Math::Vector3(0.3f, 0.3f, 0.3f));
#endif
		return entity;
	}
}