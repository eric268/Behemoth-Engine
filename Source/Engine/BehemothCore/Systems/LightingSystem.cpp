#include "pch.h"
#include "LightingSystem.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Renderer/Renderer.h"
#include "Geometry/Primitives.h"
#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Misc/CameraHelper.h"

namespace Behemoth
{
	void LightingSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
 		CameraComponent* mainCameraComp = nullptr;
		BMath::Vector3 mainCameraPosition = BMath::Vector3::Zero();

		for (const auto& [
			entityHandle, 
				cameraComp, 
				transformComp] : registry.Get<CameraComponent, TransformComponent>())
		{
			if (cameraComp->isMain)
			{
				mainCameraComp = cameraComp;
				mainCameraPosition = transformComp->worldPosition;
				break;
			}
		}

		assert(mainCameraComp);

		// Should only be one ambient light
		auto ambientLightComp = registry.Get<AmbientLightComponent>();

		// Should only be one directional light
		auto directionalComp = registry.Get<DirectionalLightComponent>();

		auto pointLightComp = registry.Get<PointLightComponent, TransformComponent>();

		for (int i = 0; i < Renderer::GetInstance().primitivesToDraw.size(); i++)
		{
			Primitive* primitive = Renderer::GetInstance().primitivesToDraw[i];
			
			if (!primitive)
			{
				LOGMESSAGE(Error, "Null primitive found");
				continue;
			}

			if (!primitive->affectedByLighting)
			{
				primitive->SetLighting(BMath::Vector3(1.0f, 1.0f, 1.0f));
				continue;
			}

			primitive->SetLighting(BMath::Vector3::Zero());

			for (const auto& [ambientEntity, ambientLightComp] : ambientLightComp)
			{
				CalculateAmbientLights(primitive, ambientLightComp);
			}

			for (const auto& [dirEntity, dirLightComp] : directionalComp)
			{
				CalculateDirectionalLights(primitive, dirLightComp, mainCameraPosition);
			}
			
			for (const auto& [pointEntity, pointLightComp, transformComp] : pointLightComp)
			{
				 CalculatePointLights(primitive, pointLightComp, mainCameraPosition, transformComp->worldPosition, mainCameraComp->viewMatrix);
			}
		}
	}

	void LightingSystem::CalculateAmbientLights(Primitive* primitive, const AmbientLightComponent* ambientLightComp)
	{
		BMath::Vector3 ambientLight = ambientLightComp->color * ambientLightComp->intensity;
		primitive->AddLighting(ambientLight);
	}

	void LightingSystem::CalculateDirectionalLights(Primitive* primitive, const DirectionalLightComponent* dirLightComp, const BMath::Vector3& cameraPos)
	{
		const BMath::Vector3 normal = primitive->normals[0];
		
		const BMath::Vector3 lightDir = BMath::Vector3::Normalize(dirLightComp->direction);
		const BMath::Vector3 primitivePos = GetPrimitivePosition(primitive);

		BMath::Vector3 diffuse = CalculateDiffuseLighting(
			normal, 
			dirLightComp->direction,
			primitive->diffuse,
			dirLightComp->color, 
			dirLightComp->intensity);

		BMath::Vector3 specular = CalculateSpecularLighting(
			primitive,
			cameraPos,
			normal,
			lightDir,
			dirLightComp->color,
			dirLightComp->intensity);

		primitive->AddLighting(diffuse + specular);
	}

	void LightingSystem::CalculatePointLights(
		Primitive* primitive, 
		const PointLightComponent* pointLightComp,
		const BMath::Vector3& cameraPos,
		const BMath::Vector3& lightPos,
		const BMath::Matrix4x4& viewMatrix)
	{
		const BMath::Vector3 normal = primitive->normals[0];

		BMath::Vector3 lightDir = lightPos - GetPrimitivePosition(primitive);
		float distance = BMath::Vector3::Magnitude(lightDir);
		lightDir.Normalize();

		BMath::Vector3 diffuse = CalculateDiffuseLighting(
			normal, 
			lightDir,
			primitive->diffuse,
			pointLightComp->color,
			pointLightComp->intensity);

		BMath::Vector3 specular = CalculateSpecularLighting(
			primitive, 
			cameraPos,
			normal, 
			lightDir, 
			pointLightComp->color,
			pointLightComp->intensity);

		float attenuation = 1.0f / (pointLightComp->constant + (pointLightComp->linear * distance) + (pointLightComp->quadratic * distance * distance));
		primitive->AddLighting((diffuse + specular) * attenuation);
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

	BMath::Vector3 LightingSystem::GetPrimitivePosition(Primitive* primitive)
	{
		int numVertices = static_cast<PrimitiveType>(primitive->primitiveType);
		if (numVertices <= 0)
		{
			return BMath::Vector3{};
		}

		BMath::Vector3 averagePos{};

		for (int i = 0; i < numVertices; i++)
		{
			averagePos += BMath::Vector3(primitive->vertices[i]) / static_cast<float>(numVertices);
		}
		return averagePos;
	}
}