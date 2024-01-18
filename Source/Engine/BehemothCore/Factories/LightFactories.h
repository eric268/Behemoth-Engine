#pragma once

#include "ECS/Entity.h"
#include "ECS/Registry.h"

#include <string>

namespace Behemoth
{
	class LightFactory
	{
	public:
		LightFactory() = default;
		ECS::EntityHandle CreatePointLight(ECS::Registry& registry, const std::string& name = "Point Light");

	private:
	};
}


