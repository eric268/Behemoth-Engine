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
		ECS::EntityHandle CreateGameObject(
			ECS::Registry& registry,
			std::string modelFilePath,
			std::string texturePath =					"",
			std::string entityName =					"Game Object",
			BMath::Vector2 uvScale =					{ 1,1 });

		ECS::EntityHandle AddChildObject(
			ECS::Registry& registry,
			const ECS::EntityHandle& parentHandle,
			std::string modelFilePath,
			std::string texturePath,
			std::string entityName						= "Game Object");

	};
}

