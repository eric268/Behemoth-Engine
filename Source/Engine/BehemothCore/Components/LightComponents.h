#pragma once
#include "ECS/Component.h"

namespace Behemoth
{
	struct AmbientLightComponent : public ECS::Component 
	{
		AmbientLightComponent() : color(BMath::Vector3(0.25f, 0.25f, 0.25f)), intensity(0.3f) {}
		AmbientLightComponent(BMath::Vector3 color, float intensity) : color(color), intensity(intensity) {}

		BMath::Vector3 color;
		float intensity;
	};

	struct DirectionalLightComponent : public ECS::Component
	{
		DirectionalLightComponent() :
			direction							(BMath::Vector3(-0.57f, -0.57f, 0.57f)),
			color								(BMath::Vector3(0.25f, 0.25f, 0.25f)),
			intensity							(4.0f)
		{}
												
		DirectionalLightComponent(
			BMath::Vector3 dir,
			BMath::Vector3 color,
			float intensity) 
			:
			direction							(dir),
			color								(color),
			intensity							(intensity) 
		{}

		BMath::Vector3 direction;
		BMath::Vector3 color;
		float intensity;
	};

	struct PointLightComponent : public ECS::Component
	{
		PointLightComponent() :
			color								(BMath::Vector3(0.25f, 0.25, 0.25f)),
			intensity							(1.0f),
			constant							(1.0f),
			linear								(0.14f),
			quadratic							(0.07f)
		{}
																								
		PointLightComponent(
			BMath::Vector3 color,
			float intensity,
			float constant  =					1.0f,
			float linear    =					0.14f,
			float quadratic =					0.07f) 
			:
			color								(color),
			intensity							(intensity),
			constant							(constant),
			linear								(linear),
			quadratic							(quadratic)
		{}

		BMath::Vector3 color;
		float intensity;
		float constant;
		float linear;
		float quadratic;
	};
}