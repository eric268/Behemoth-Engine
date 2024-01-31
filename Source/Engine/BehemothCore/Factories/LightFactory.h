#pragma once

#include "Math/Vector.h"

namespace ECS
{
	class Registry;
	struct EntityHandle;
}

namespace Behemoth
{
	struct LightFactory
	{
		static ECS::EntityHandle CreatePointLight(
			ECS::Registry& registry,
			const BMath::Vector3& location,
			const BMath::Vector3& color,
			float intensity,
			const std::string& name = "Point Light",
			const std::string& modelName = "",
			const std::string& texture = "");
	};
}


