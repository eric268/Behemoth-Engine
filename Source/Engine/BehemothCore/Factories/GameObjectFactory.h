#pragma once

#include "Math/Vector.h"

namespace ECS
{
	class Registry;
	struct EntityHandle;
}

namespace Behemoth
{
	struct GameObjectFactory
	{
		static ECS::EntityHandle CreateGameObject(
			ECS::Registry& registry,
			const std::string& modelFilePath,
			const std::string& texturePath =	"",
			const std::string& entityName = "Game Object",
			BMath::Vector2 uvScale = BMath::Vector2(1.0f, 1.0f));
	};
}

