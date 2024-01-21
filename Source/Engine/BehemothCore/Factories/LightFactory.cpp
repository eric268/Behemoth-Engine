#include "pch.h"
#include "LightFactory.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Components/LightComponents.h"

namespace Behemoth
{
	ECS::EntityHandle LightFactory::CreatePointLight(
		ECS::Registry& registry,
		const BMath::Vector3& location,
		const BMath::Vector3& color,
		float intensity,
		const std::string& texture,
		bool showMesh,
		const std::string& name)
	{
		ECS::EntityHandle entity = registry.CreateEntity(name);

		registry.AddComponent<PointLightComponent>(entity, color, intensity);
		registry.AddComponent<TransformComponent>(entity);
		registry.AddComponent<MoveComponent>(entity, location);

		registry.AddComponent<MeshComponent>(entity, "cube.obj", texture, BMath::Vector2(0.2f, 0.2f), true, false);
		registry.AddComponent<ScalingComponent>(entity, BMath::Vector3(.2f, .2f, .2f));
		registry.AddComponent<MeshInitalizeComponent>(entity, false, false);

		return entity;
	}
}