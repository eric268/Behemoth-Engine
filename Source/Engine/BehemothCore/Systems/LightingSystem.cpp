#include "pch.h"
#include "LightingSystem.h"
#include "Render/Renderer.h"

#include "Components/Components.h"
#include "Misc/CameraHelper.h"

 #define ENABLE_LIGHTING

// stl
#include <algorithm>
#include <iostream>
#include <string>

namespace Behemoth
{
	void LightingSystem::Run(ECS::Registry& registry)
	{
 		CameraComponent* mainCamera = CameraHelper::GetMainCamera(registry);
		Math::Vector3 mainCameraPosition = CameraHelper::GetMainCameraPostition(registry);

		auto directionalComponent = registry.Get<DirectionalLightComponent>();
		auto pointLightComponents = registry.Get<PointLightComponent, TransformComponent>();

		for (int i = 0; i < Renderer::GetInstance().primitivesToDraw.size(); i++)
		{
			Primitives* primitive = Renderer::GetInstance().primitivesToDraw[i];
			assert(primitive);

#ifdef ENABLE_LIGHTING
			primitive->SetLighting(Math::Vector3::Zero());

			// Should only be one directional light but for now continue with possibility of multiple
			for (const auto& [dirEntity, dirLight] : directionalComponent)
			{
				CalculateDirectionalLights(primitive, dirLight, mainCameraPosition);
			}
			
			for (const auto& [pointEntity, pointLight, transformComp] : pointLightComponents)
			{
				CalculatePointLights(primitive, pointLight, mainCameraPosition, transformComp->position, mainCamera->viewMatrix);
			}
#else
			primitive->SetLighting(Math::Vector3(1.0f, 1.0f, 1.0f));
#endif
		}
	}

	void LightingSystem::CalculateDirectionalLights(Primitives* primitive, const DirectionalLightComponent* light, const Math::Vector3& cameraPos)
	{
		const Math::Vector3 normal = primitive->normals[0];
		const Math::Vector3 lightDir = Math::Vector3::Normalize(light->direction);
		const Math::Vector3 primitivePos = GetPrimitivePosition(primitive);

		Math::Vector3 diffuse = CalculateDiffuseLighting(normal, light->direction, primitive->diffuse, light->color, light->intensity);
		Math::Vector3 specular = CalculateSpecularLighting(primitive, cameraPos, normal, lightDir, light->color, light->intensity);

		primitive->AddLighting(diffuse + specular);
	}

	void LightingSystem::CalculatePointLights(Primitives* primitive, const PointLightComponent* light, const Math::Vector3& cameraPos, const Math::Vector3& lightPos, const Math::Matrix4x4& viewMatrix)
	{
		const Math::Vector3 normal = primitive->normals[0];
		Math::Vector3 lightDir = lightPos - GetPrimitivePosition(primitive);
		float distance = Math::Vector3::Magnitude(lightDir);
		lightDir.Normalize();
		// std::cout << "Distance: " << std::to_string(distance) << std::endl;
		// std::cout << "Direction X: " << std::to_string(lightDir.x ) << " Y: " << std::to_string(lightDir.y) << " Z: " << std::to_string(lightDir.y) << std::endl;

		Math::Vector3 diffuse = CalculateDiffuseLighting(normal, lightDir, primitive->diffuse, light->color, light->intensity);
		Math::Vector3 specular = CalculateSpecularLighting(primitive, cameraPos, normal, lightDir, light->color, light->intensity);

		float attenuation = 1.0 / (light->constant + (light->linear * distance) + (light->quadratic * distance * distance));
		primitive->AddLighting((diffuse + specular) * attenuation);
	}

	Math::Vector3 LightingSystem::GetPrimitivePosition(Primitives* primitive)
	{
		int numVertices = static_cast<PrimitiveType>(primitive->primitiveType);
		if (numVertices <= 0) 
			return Math::Vector3{};

		Math::Vector3 averagePos{};

		for (int i = 0; i < numVertices; i++) 
		{
			averagePos += Math::Vector3(primitive->verticies[i]) / static_cast<float>(numVertices);
		}
		return averagePos;
	}

	Math::Vector3 LightingSystem::CalculateDiffuseLighting(const Math::Vector3& surfaceNormal,
														   const Math::Vector3& lightDir,
														   const Math::Vector3& primitiveDiffusion,
														   const Math::Vector3& lightColor,
														   const float lightIntensity)
	{
		float diff = std::max(Math::Vector3::Dot(surfaceNormal, lightDir), 0.0f);
		return primitiveDiffusion * diff * lightIntensity * lightColor;
	}

	Math::Vector3 LightingSystem::CalculateSpecularLighting(Primitives* primitive,
														    const Math::Vector3& cameraPos,
														    const Math::Vector3& surfaceNormal,
														    const Math::Vector3& lightDir,
														    const Math::Vector3& lightColor,
														    const float lightIntensity)
	{
		Math::Vector3 primitivePos = GetPrimitivePosition(primitive);
		Math::Vector3 viewDir = Math::Vector3::Normalize(primitivePos - cameraPos);
		Math::Vector3 reflectDir = Math::Vector3::Reflect(lightDir, surfaceNormal);
		
		float spec = std::pow(std::max(Math::Vector3::Dot(viewDir, reflectDir), 0.0f), primitive->shininess);
		return primitive->specular * lightColor * lightIntensity * spec;
	}
}