#pragma once

namespace ECS
{
	class Registry;
	struct EntityHandle;
}

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

