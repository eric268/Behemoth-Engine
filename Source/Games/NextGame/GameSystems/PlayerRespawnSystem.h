#pragma once
#include "BehemothEngine.h"

namespace ECS
{
	class Registry;
	class Entity;
}

class PlayerRespawnSystem
{
public:
		void Run(const float deltaTime, ECS::Registry& registry);

private:

};

