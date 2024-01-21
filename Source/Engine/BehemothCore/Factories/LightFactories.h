#pragma once

#include "ECS/Entity.h"
#include "ECS/Registry.h"
#include "Math/Vector.h"

#include <string>

namespace Behemoth
{
	struct LightFactory
	{
		static ECS::EntityHandle CreatePointLight(ECS::Registry& registry, const BMath::Vector3& color, float intensity, bool showMesh = true, const std::string& name = "Point Light");
	};
}


