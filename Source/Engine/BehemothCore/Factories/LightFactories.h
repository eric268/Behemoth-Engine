#pragma once

#include "ECS/Entity.h"
#include "ECS/Registry.h"

#include <string>

namespace Behemoth
{
	struct LightFactory
	{
		ECS::EntityHandle CreatePointLight(ECS::Registry& registry, const std::string& name = "Point Light");
	};
}


