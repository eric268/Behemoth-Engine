#pragma once
#include "ECS/Entity.h"
#include "Math/Vector.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct SkySphereFactory
	{
		static ECS::EntityHandle CreateSkySphere(ECS::Registry& registry, const std::string& texturePath, const BMath::Vector2& uvScale);
	};
}