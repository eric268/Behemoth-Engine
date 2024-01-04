#pragma once

#include "ECS/ECSCore.h"
#include <string>

namespace Behemoth
{
	class GameObjectFactory
	{
	public:
		ECS::EntityHandle CreateGameObject(ECS::Registry& registry, std::string modelFilePath, std::string texturePath = "", std::string entityName = "Game Object");

	private:

	};
}

