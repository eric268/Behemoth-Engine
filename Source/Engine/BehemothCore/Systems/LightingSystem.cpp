#include "LightingSystem.h"
#include "Render/Renderer.h"

#include "Components/Components.h"
#include "Misc/CameraHelper.h"

// stl
#include <algorithm>

namespace Behemoth
{
	void LightingSystem::Run(ECS::Registry& registry)
	{
 		CameraComponent* mainCamera = CameraHelper::GetMainCamera(registry);

		auto directionalComponent = registry.Get<DirectionalLightComponent>();
		assert(directionalComponent.size() > 0);

		auto [directionalEntity, directionalLight] = directionalComponent[0];
		auto pointLightComponents = registry.Get<DirectionalLightComponent>();


		CalculateDirectionalLights(directionalLight, mainCamera->viewMatrix);		
	}

	void LightingSystem::CalculateDirectionalLights(const DirectionalLightComponent* light, const Math::Matrix4x4& viewMatrix)
	{
		for (int i = 0; i < Renderer::GetInstance().primitivesToDraw.size(); i++)
		{
			if (!Renderer::GetInstance().primitivesToDraw[i])
				continue;

			float diff = std::max(Math::Vector3::Dot(Renderer::GetInstance().primitivesToDraw[i]->normals[0],
				Math::Vector3(-light->direction.x, -light->direction.y, -light->direction.z)), 0.0f);

// 			if (diff == 0.0f)
// 				continue;

			Math::Vector3 diffuse = Renderer::GetInstance().primitivesToDraw[i]->diffuse * diff * light->intensity;
			diffuse.x *= light->color.x;
			diffuse.y *= light->color.y;
			diffuse.z *= light->color.z;
	
			Renderer::GetInstance().primitivesToDraw[i]->SetLighting(diffuse);
		}
		
	}
	void LightingSystem::CalculatePointLights(const PointLightComponent* light, const Math::Matrix4x4& viewMatrix)
	{

	}
}