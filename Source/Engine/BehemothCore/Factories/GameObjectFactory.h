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
			BMath::Vector2 uvScale = { 1,1 });

		static bool AddChildObject(
			ECS::Registry& registry,
			const ECS::EntityHandle& parentHandle,
			const ECS::EntityHandle& childHandle);
	};
}

