#pragma once

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct TextSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);
	};
}

