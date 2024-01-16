#pragma once

namespace ECS
{
	class Registry;
}
namespace Behemoth
{
	class AudioSystem
	{
	public:

		void Run(const float deltaTime, ECS::Registry& registry);
	};
}

