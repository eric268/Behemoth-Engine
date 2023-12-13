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
		auto pointLightComponents = registry.Get<PointLightComponent, TransformComponent>();

		// Should only be one directional light but for now continue with possibility of multiple
		for (const auto& [dirEntity, dirLight] : directionalComponent)
		{
			CalculateDirectionalLights(dirLight, mainCamera->viewMatrix);
		}

		for (const auto& [pointEntity, pointLight, transformComp] : pointLightComponents)
		{
			CalculatePointLights(pointLight, transformComp->position, mainCamera->viewMatrix);
		}
	}

	void LightingSystem::CalculateDirectionalLights(const DirectionalLightComponent* light, const Math::Matrix4x4& viewMatrix)
	{
		for (int i = 0; i < Renderer::GetInstance().primitivesToDraw.size(); i++)
		{
			Primitives* primitive = Renderer::GetInstance().primitivesToDraw[i];

			if (!primitive)
				continue;
			
			const Math::Vector3 normal = primitive->normals[0];

			//Diffuse Light
			Math::Vector3 negativeLightDirection = Math::Vector3(-light->direction.x, -light->direction.y, -light->direction.z);
			negativeLightDirection.Normalize();
			float diff = std::max(Math::Vector3::Dot(normal, negativeLightDirection), 0.0f);
			Math::Vector3 diffuse = primitive->diffuse * diff * light->intensity;
			diffuse *= light->color;

// 			// Specular Light
			Math::Vector3 primitivePos = GetPrimitivePosition(primitive);
			Math::Vector3 cameraPos = Math::Vector3(viewMatrix[3][0], viewMatrix[3][1], viewMatrix[3][2]);
			Math::Vector3 viewDir = Math::Vector3::Normalize(cameraPos - primitivePos);
			Math::Vector3 reflectDir = Math::Vector3::Reflect(light->direction, normal);

			float shininess = primitive->shininess;
			float spec = std::pow(std::max(Math::Vector3::Dot(viewDir, reflectDir), 0.0f), shininess);
			Math::Vector3 specular = primitive->specular * light->color * light->intensity * spec;
	
			// primitive->SetLighting(diffuse);
			primitive->SetLighting(diffuse + specular);
		}
	}
	void LightingSystem::CalculatePointLights(const PointLightComponent* light, const Math::Vector3& position, const Math::Matrix4x4& viewMatrix)
	{
		for (int i = 0; i < Renderer::GetInstance().primitivesToDraw.size(); i++)
		{
			Primitives* primitive = Renderer::GetInstance().primitivesToDraw[i];

			if (!primitive)
				continue;

			const Math::Vector3 normal = primitive->normals[0];
			Math::Vector3 lightDir = position -  GetPrimitivePosition(primitive);
			float distance = Math::Vector3::Magnitude(lightDir);
			lightDir.Normalize();

			float attenuation = 1.0 / (light->constant + (light->linear * distance) + (light->quadratic * distance * distance));

			float diff = std::max(Math::Vector3::Dot(normal, lightDir), 0.0f);
			Math::Vector3 diffuse = primitive->diffuse * light->color * light->intensity * diff;

 			// Specular Light
			Math::Vector3 primitivePos = GetPrimitivePosition(primitive);
			Math::Vector3 cameraPos = Math::Vector3(viewMatrix[3][0], viewMatrix[3][1], viewMatrix[3][2]);
			Math::Vector3 viewDir = Math::Vector3::Normalize(cameraPos - primitivePos);
			Math::Vector3 negLightDir = Math::Vector3(-lightDir.x, -lightDir.y, -lightDir.z);
			Math::Vector3 reflectDir = Math::Vector3::Reflect(negLightDir, normal);

			float shininess = primitive->shininess;
			float spec = std::pow(std::max(Math::Vector3::Dot(viewDir, reflectDir), 0.0f), shininess);
			Math::Vector3 specular = primitive->specular * light->color * light->intensity * spec;

			//primitive->SetLighting(diffuse * attenuation);
			primitive->AddLighting((diffuse + specular) * attenuation);
		}
	}

	Math::Vector3 LightingSystem::GetPrimitivePosition(Primitives* primitive)
	{
		int numVerticies = static_cast<PrimitiveType>(primitive->primitiveType);
		Math::Vector3 averagePos{};

		for (int i = 0; i < numVerticies; i++)
		{
			averagePos += primitive->verticies[i];
		}
		averagePos.x /= numVerticies;
		averagePos.y /= numVerticies;
		averagePos.z /= numVerticies;
		return averagePos;
	}
}