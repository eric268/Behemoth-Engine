#pragma once

namespace ECS
{
	class Registry;
}

class PCSystem
{
public:
	void Run(const float deltaTime, ECS::Registry& registry);

private:

};

