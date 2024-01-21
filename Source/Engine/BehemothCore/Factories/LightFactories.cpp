#include "pch.h"
#include "LightFactories.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Components/LightComponents.h"

namespace Behemoth
{
	ECS::EntityHandle LightFactory::CreatePointLight(ECS::Registry& registry, const BMath::Vector3& color, float intensity, bool showMesh, const std::string& name)
	{
		ECS::EntityHandle entity = registry.CreateEntity(name);

		registry.AddComponent<PointLightComponent>(entity, color, intensity);
		registry.AddComponent<TransformComponent>(entity);

		// Just used for debugging lights to visualize where they are in the world 
// #ifdef DEBUG
		registry.AddComponent<MeshComponent>(entity, "cube.obj", "light.png", BMath::Vector2(0.2f, 0.2f), true);
		registry.AddComponent<ScalingComponent>(entity, BMath::Vector3(.2f, .2f, .2f));
		registry.AddComponent<MeshInitalizeComponent>(entity, false, false);
// #endif
		return entity;
	}
}