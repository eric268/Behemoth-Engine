#include "pch.h"
#include "LightFactories.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Components/LightComponents.h"

namespace Behemoth
{

	ECS::EntityHandle LightFactory::CreatePointLight(ECS::Registry& registry, const std::string& name)
	{
		ECS::EntityHandle entity = registry.CreateEntity(name);

		registry.AddComponent<PointLightComponent>(entity, BMath::Vector3(0.75f, 0.75f, 0.75f), 1.0f);
		registry.AddComponent<TransformComponent>(entity);
		registry.AddComponent<MoveComponent>(entity, BMath::Vector3(0.0f, 0.0f, 0.0f));

		// Just used for debugging lights to visualize where they are in the world 
#ifdef DEBUG
		// registry.AddComponent<MeshComponent>(entity, "cube.obj", "brick.png", BMath::Vector2(0.5f, 0.5f), false);
		// registry.AddComponent<ScalingComponent>(entity, BMath::Vector3(.2f, .2f, .2f));
		// registry.AddComponent<MeshInitalizeComponent>(entity, false, false);
#endif
		return entity;
	}
}