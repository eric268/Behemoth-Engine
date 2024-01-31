#include "pch.h"
#include "LightFactory.h"
#include "ECS/Entity.h"
#include "ECS/Registry.h"
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
		const std::string& name,
		const std::string& modelName,
		const std::string& texture)
	{
		ECS::EntityHandle entity = registry.CreateEntity(name);

		registry.AddComponent<PointLightComponent>(entity, color, intensity);
		registry.AddComponent<TransformComponent>(entity);
		registry.AddComponent<MoveComponent>(entity, location);

		if (modelName != "")
		{
			registry.AddComponent<MeshComponent>(
				entity, 
				modelName, 
				texture, 
				BMath::Vector2(1.0f, 1.0f),
				true,
				false);

			registry.AddComponent<MeshInitializeComponent>(entity);
		}

		return entity;
	}
}