#pragma once

#include "Math/MathCore.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct MeshComponent;
	struct AmbientLightComponent;
	struct DirectionalLightComponent;
	struct PointLightComponent;
	struct TransformComponent;
	class Primitive;
	
	class LightingSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void CalculateAmbientLights(Primitive* primitive, const AmbientLightComponent* light);
		void CalculateDirectionalLights(Primitive* primitive, const DirectionalLightComponent* light, const BMath::Vector3& cameraPos);
		void CalculatePointLights(Primitive* primitive, const PointLightComponent* light, const BMath::Vector3& cameraPos, const BMath::Vector3& lightPos, const BMath::Matrix4x4& viewMatrix);
		BMath::Vector3 GetPrimitivePosition(Primitive* primitive);

		BMath::Vector3 CalculateDiffuseLighting(const BMath::Vector3& surfaceNormal,
											   const BMath::Vector3& lightDir, 
											   const BMath::Vector3& primitiveDiffusion, 
											   const BMath::Vector3& lightColor, 
											   const float lightIntensity);

		BMath::Vector3 CalculateSpecularLighting(Primitive* primitive,
												const BMath::Vector3& cameraPos,
												const BMath::Vector3& surfaceNormal,
											    const BMath::Vector3& lightDir, 
											    const BMath::Vector3& lightColor,
											    const float lightIntensity);
	};
}
