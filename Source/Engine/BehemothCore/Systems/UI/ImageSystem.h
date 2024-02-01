#pragma once

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct ImageSystem
	{
		void Run(const float deltaTime, ECS::Registry& registry);
	};
}

