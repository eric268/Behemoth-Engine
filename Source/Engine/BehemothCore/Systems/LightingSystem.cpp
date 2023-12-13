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
		Math::Vector3 mainCameraPosition = CameraHelper::GetMainCameraPostition(registry);

		auto directionalComponent = registry.Get<DirectionalLightComponent>();
		auto pointLightComponents = registry.Get<PointLightComponent, TransformComponent>();

		// Should only be one directional light but for now continue with possibility of multiple
		for (const auto& [dirEntity, dirLight] : directionalComponent)
		{
			CalculateDirectionalLights(dirLight, mainCameraPosition);
		}

		for (const auto& [pointEntity, pointLight, transformComp] : pointLightComponents)
		{
			CalculatePointLights(pointLight, mainCameraPosition, transformComp->position, mainCamera->viewMatrix);
		}
	}

	void LightingSystem::CalculateDirectionalLights(const DirectionalLightComponent* light, const Math::Vector3& cameraPos)
	{
		for (int i = 0; i < Renderer::GetInstance().primitivesToDraw.size(); i++)
		{
			Primitives* primitive = Renderer::GetInstance().primitivesToDraw[i];
			assert(primitive);
			
			const Math::Vector3 normal = primitive->normals[0];
			const Math::Vector3 lightDir = Math::Vector3::Normalize(light->direction);
			const Math::Vector3 primitivePos = GetPrimitivePosition(primitive);

			Math::Vector3 diffuse  = CalculateDiffuseLighting(normal, light->direction, primitive->diffuse, light->color, light->intensity);
			Math::Vector3 specular = CalculateSpecularLighting(primitive, cameraPos, normal, lightDir, light->color, light->intensity);
	
			primitive->SetLighting(diffuse + specular);
		}
	}
	void LightingSystem::CalculatePointLights(const PointLightComponent* light, const Math::Vector3& cameraPos, const Math::Vector3& lightPos, const Math::Matrix4x4& viewMatrix)
	{
		for (int i = 0; i < Renderer::GetInstance().primitivesToDraw.size(); i++)
		{
			Primitives* primitive = Renderer::GetInstance().primitivesToDraw[i];
			assert(primitive);

			const Math::Vector3 normal = primitive->normals[0];
			Math::Vector3 lightDir = lightPos -  GetPrimitivePosition(primitive);
			float distance = Math::Vector3::Magnitude(lightDir);
			lightDir.Normalize();

			Math::Vector3 diffuse  = CalculateDiffuseLighting(normal, lightDir, primitive->diffuse, light->color, light->intensity);
			Math::Vector3 specular = CalculateSpecularLighting(primitive, cameraPos, normal, lightDir, light->color, light->intensity);

			float attenuation = 1.0 / (light->constant + (light->linear * distance) + (light->quadratic * distance * distance));
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

	Math::Vector3 LightingSystem::CalculateDiffuseLighting(const Math::Vector3& surfaceNormal,
														   const Math::Vector3& lightDir,
														   const Math::Vector3& primitiveDiffusion,
														   const Math::Vector3& lightColor,
														   const float lightIntensity)
	{
		//Diffuse Light
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
		Math::Vector3 viewDir = Math::Vector3::Normalize(cameraPos - primitivePos);
		Math::Vector3 reflectDir = Math::Vector3::Reflect(lightDir, surfaceNormal);
		float spec = std::pow(std::max(Math::Vector3::Dot(viewDir, reflectDir), 0.0f), primitive->shininess);
		return primitive->specular * lightColor * lightIntensity * spec;
	}
}