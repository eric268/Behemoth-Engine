#pragma once

namespace ECS
{
	class Registry;
	class EntityHandle;
}

namespace Behemoth
{
	struct TransformSystem
	{
		void Run(const float deltaTime, ECS::Registry& registry);
	};
}