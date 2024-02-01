#pragma once

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct TimerSystem
	{
		void Run(const float deltaTime, ECS::Registry& registry);
	};
}
