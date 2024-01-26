#pragma once

#include "Math/Vector.h"
#include <string>

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
			std::string modelFilePath,
			std::string texturePath =	"",
			std::string entityName = "Game Object",
			BMath::Vector2 uvScale = BMath::Vector2(1.0f, 1.0f));

		static bool AddChildObject(
			ECS::Registry& registry,
			const ECS::EntityHandle& parentHandle,
			const ECS::EntityHandle& childHandle);
	};
}

