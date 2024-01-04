#pragma once

#include "ECS/Entity.h"
#include "ECS/Registry.h"

#include <string>

namespace Behemoth
{
	class DirectionalLightFactory
	{
	public:
		DirectionalLightFactory() = default;
		ECS::EntityHandle CreateDirectionalLight(ECS::Registry& registry, const std::string& name = "Directional Light");

	private:

	};

	class PointLightFactory
	{
	public:
		PointLightFactory() = default;
		ECS::EntityHandle CreatePointLight(ECS::Registry& registry, const std::string& name = "Point Light");

	private:

	};
}


