#pragma once

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class ImageSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
	};
}

