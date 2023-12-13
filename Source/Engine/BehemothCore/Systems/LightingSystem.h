#pragma once

#include "ECS/System.h"
#include "Math/MathCore.h"

#include <vector>

namespace Behemoth
{
	class MeshComponent;
	class DirectionalLightComponent;
	class PointLightComponent;
	class TransformComponent;
	class Primitives;
	
	class LightingSystem : ECS::System
	{
	public:
		virtual void Run(ECS::Registry& registry) override;

	private:
		void CalculateDirectionalLights(const DirectionalLightComponent* light, const Math::Matrix4x4& viewMatrix);
		void CalculatePointLights(const PointLightComponent* light, const Math::Vector3& position, const Math::Matrix4x4& viewMatrix);
		Math::Vector3 GetPrimitivePosition(Primitives* primitive);
	};
}
