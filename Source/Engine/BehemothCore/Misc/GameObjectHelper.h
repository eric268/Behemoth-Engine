#pragma once
#include "ECS/Registry.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	struct GameObjectHelper
	{
		static bool AddChildObject(
			ECS::Registry& registry,
			const ECS::EntityHandle& parentHandle,
			const ECS::EntityHandle& childHandle);
	};
}

