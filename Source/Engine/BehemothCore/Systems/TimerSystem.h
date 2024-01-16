#pragma once

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class TimerSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);
	};
}
