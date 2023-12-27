#include "pch.h"
#include "LightFactories.h"
#include "Components/Components.h"

namespace Behemoth
{
	ECS::Entity DirectionalLightFactory::CreateDirectionalLight(ECS::Registry& registry, const std::string& name)
	{
		ECS::Entity entity = registry.CreateEntity(name);
		registry.AddComponent<Behemoth::DirectionalLightComponent>(entity, Math::Vector3(-0.707f, 0, -0.707f),
			Math::Vector3(0.25f, 0.25f, 0.25f), 1.5f);
		return entity;
	}

	ECS::Entity PointLightFactory::CreatePointLight(ECS::Registry& registry, const std::string& name)
	{
		ECS::Entity entity = registry.CreateEntity(name);

		registry.AddComponent<Behemoth::PointLightComponent>(entity, Math::Vector3(0.75f, 0.75f, 0.75f), 1.0f);
		registry.AddComponent<Behemoth::TransformComponent>(entity);
		registry.AddComponent<Behemoth::MoveComponent>(entity, Math::Vector3(0.0f, 0.0f, 0.0f));
#ifdef DEBUG
		registry.AddComponent<Behemoth::MeshComponent>(entity, "cube.obj", "brick.png", Math::Vector2(0.5f, 0.5f), false);
		registry.AddComponent<Behemoth::ScalingComponent>(entity, Math::Vector3(0.3f, 0.3f, 0.3f));
#endif
		return entity;
	}
}