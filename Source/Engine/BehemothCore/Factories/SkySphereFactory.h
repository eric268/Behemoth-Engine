#pragma once

#include "Math/Vector.h"

namespace ECS
{
	class Registry;
	struct EntityHandle;
}

namespace Behemoth
{
	struct SkySphereFactory
	{
		static ECS::EntityHandle CreateSkySphere(ECS::Registry& registry, const std::string& texturePath, const float sphereScale, const BMath::Vector2& uvScale);
	};
}