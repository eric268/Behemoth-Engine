#pragma once

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class TextSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
	};
}

