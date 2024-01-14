#pragma once

#include "ECS/System.h"
#include "Math/MathCore.h"

#include <vector>

namespace Behemoth
{
	struct MeshComponent;
	struct AmbientLightComponent;
	struct DirectionalLightComponent;
	struct PointLightComponent;
	struct TransformComponent;
	class Primitives;
	
	class LightingSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void CalculateAmbientLights(Primitives* primitive, const AmbientLightComponent* light);
		void CalculateDirectionalLights(Primitives* primitive, const DirectionalLightComponent* light, const BMath::Vector3& cameraPos);
		void CalculatePointLights(Primitives* primitive, const PointLightComponent* light, const BMath::Vector3& cameraPos, const BMath::Vector3& lightPos, const BMath::Matrix4x4f& viewMatrix);
		BMath::Vector3 GetPrimitivePosition(Primitives* primitive);

		BMath::Vector3 CalculateDiffuseLighting(const BMath::Vector3& surfaceNormal,
											   const BMath::Vector3& lightDir, 
											   const BMath::Vector3& primitiveDiffusion, 
											   const BMath::Vector3& lightColor, 
											   const float lightIntensity);

		BMath::Vector3 CalculateSpecularLighting(Primitives* primitive,
												const BMath::Vector3& cameraPos,
												const BMath::Vector3& surfaceNormal,
											    const BMath::Vector3& lightDir, 
											    const BMath::Vector3& lightColor,
											    const float lightIntensity);
	};
}
