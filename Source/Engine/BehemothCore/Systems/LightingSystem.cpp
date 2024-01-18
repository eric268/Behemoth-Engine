#include "pch.h"
#include "LightingSystem.h"
#include "Renderer/Renderer.h"

#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Misc/CameraHelper.h"

// #define ENABLE_LIGHTING

// stl
#include <algorithm>
#include <iostream>
#include <string>

namespace Behemoth
{
	void LightingSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
 		CameraComponent* mainCamera = CameraHelper::GetMainCamera(registry);
		BMath::Vector3 mainCameraPosition = CameraHelper::GetMainCameraPostition(registry);

		// Should only be one ambient light
		auto ambientLightComponent = registry.Get<AmbientLightComponent>();

		// Should only be one directional light
		auto directionalComponent = registry.Get<DirectionalLightComponent>();


		auto pointLightComponents = registry.Get<PointLightComponent, TransformComponent>();

		for (int i = 0; i < Renderer::GetInstance().primitivesToDraw.size(); i++)
		{
			Primitive* primitive = Renderer::GetInstance().primitivesToDraw[i];
			assert(primitive);

#ifdef ENABLE_LIGHTING
			primitive->SetLighting(BMath::Vector3::Zero());

			for (const auto& [ambientEntity, ambientLight] : ambientLightComponent)
			{
				CalculateAmbientLights(primitive, ambientLight);
			}

			// Should only be one directional light but for now continue with possibility of multiple
			for (const auto& [dirEntity, dirLight] : directionalComponent)
			{
				CalculateDirectionalLights(primitive, dirLight, mainCameraPosition);
			}
			
			for (const auto& [pointEntity, pointLight, transformComp] : pointLightComponents)
			{
				 CalculatePointLights(primitive, pointLight, mainCameraPosition, transformComp->worldPosition, mainCamera->viewMatrix);
			}
#else
			primitive->SetLighting(BMath::Vector3(1.0f, 1.0f, 1.0f));
#endif
		}
	}

	void LightingSystem::CalculateAmbientLights(Primitive* primitive, const AmbientLightComponent* light)
	{
		BMath::Vector3 ambientLight = light->color * light->intensity;
		primitive->AddLighting(ambientLight);
	}

	void LightingSystem::CalculateDirectionalLights(Primitive* primitive, const DirectionalLightComponent* light, const BMath::Vector3& cameraPos)
	{
		const BMath::Vector3 normal = primitive->normals[0];
		
		const BMath::Vector3 lightDir = BMath::Vector3::Normalize(light->direction);
		const BMath::Vector3 primitivePos = GetPrimitivePosition(primitive);

		BMath::Vector3 diffuse = CalculateDiffuseLighting(normal, light->direction, primitive->diffuse, light->color, light->intensity);
		BMath::Vector3 specular = CalculateSpecularLighting(primitive, cameraPos, normal, lightDir, light->color, light->intensity);

		primitive->AddLighting(diffuse + specular);
	}

	void LightingSystem::CalculatePointLights(Primitive* primitive, const PointLightComponent* light, const BMath::Vector3& cameraPos, const BMath::Vector3& lightPos, const BMath::Matrix4x4f& viewMatrix)
	{
		const BMath::Vector3 normal = primitive->normals[0];

		BMath::Vector3 lightDir = lightPos - GetPrimitivePosition(primitive);
		float distance = BMath::Vector3::Magnitude(lightDir);
		lightDir.Normalize();

		BMath::Vector3 diffuse = CalculateDiffuseLighting(normal, lightDir, primitive->diffuse, light->color, light->intensity);
		BMath::Vector3 specular = CalculateSpecularLighting(primitive, cameraPos, normal, lightDir, light->color, light->intensity);

		float attenuation = 1.0f / (light->constant + (light->linear * distance) + (light->quadratic * distance * distance));
		primitive->AddLighting((diffuse + specular) * attenuation);
	}

	BMath::Vector3 LightingSystem::GetPrimitivePosition(Primitive* primitive)
	{
		int numVertices = static_cast<PrimitiveType>(primitive->primitiveType);
		if (numVertices <= 0) 
			return BMath::Vector3{};

		BMath::Vector3 averagePos{};

		for (int i = 0; i < numVertices; i++) 
		{
			averagePos += BMath::Vector3(primitive->verticies[i]) / static_cast<float>(numVertices);
		}
		return averagePos;
	}

	BMath::Vector3 LightingSystem::CalculateDiffuseLighting(const BMath::Vector3& surfaceNormal,
														   const BMath::Vector3& lightDir,
														   const BMath::Vector3& primitiveDiffusion,
														   const BMath::Vector3& lightColor,
														   const float lightIntensity)
	{
		float diff = std::max(BMath::Vector3::Dot(surfaceNormal, lightDir), 0.0f);
		return primitiveDiffusion * diff * lightIntensity * lightColor;
	}

	BMath::Vector3 LightingSystem::CalculateSpecularLighting(Primitive* primitive,
														    const BMath::Vector3& cameraPos,
														    const BMath::Vector3& surfaceNormal,
														    const BMath::Vector3& lightDir,
														    const BMath::Vector3& lightColor,
														    const float lightIntensity)
	{
		BMath::Vector3 primitivePos = GetPrimitivePosition(primitive);
		BMath::Vector3 viewDir = BMath::Vector3::Normalize(primitivePos - cameraPos);
		BMath::Vector3 reflectDir = BMath::Vector3::Reflect(lightDir, surfaceNormal);
		
		float spec = std::pow(std::max(BMath::Vector3::Dot(viewDir, reflectDir), 0.0f), primitive->shininess);
		return primitive->specular * lightColor * lightIntensity * spec;
	}
}