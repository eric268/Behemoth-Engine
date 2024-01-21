#pragma once

namespace ECS
{
	class Registry;

}

struct MovingObsSystem
{
	void Run(const float deltaTime, ECS::Registry& registry);
};

