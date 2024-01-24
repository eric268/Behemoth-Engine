#pragma once
#include "BehemothEngine.h"

namespace ECS
{
	class Registry;
	class Entity;
}

struct PlayerRespawnSystem
{
	void Run(const float deltaTime, ECS::Registry& registry);
};

