#pragma once

#include "ECS/Component.h"
#include "Input/InputCodes.h"
#include "Geometry/Mesh.h"

#include <functional>
#include <unordered_map>

namespace Behemoth
{
	struct DirectionalLightComponent : public ECS::Component
	{
		DirectionalLightComponent() :
			direction(Math::Vector3(-0.57f, -0.57f, 0.57f)),
			color(Math::Vector3(0.25f, 0.25f, 0.25f)),
			intensity(4.0f) {}

		DirectionalLightComponent(Math::Vector3 dir, Math::Vector3 color, float intensity) :
			direction(dir),
			color(color),
			intensity(intensity) {}

		Math::Vector3 direction;
		Math::Vector3 color;
		float intensity;
	};

	struct PointLightComponent : public ECS::Component
	{
		PointLightComponent() :
			color(Math::Vector3(0.25f, 0.25, 0.25f)),
			intensity(1.0f),
			constant(1.0f),
			linear(0.14f),
			quadratic(0.07f)
		{}

		PointLightComponent(Math::Vector3 color, float intensity, float constant = 1.0f, float linear = 0.14f, float quadratic = 0.07f) :
			color(color),
			intensity(intensity),
			constant(constant),
			linear(linear),
			quadratic(quadratic)
		{}

		Math::Vector3 color;
		float intensity;
		float constant;
		float linear;
		float quadratic;
	};
}