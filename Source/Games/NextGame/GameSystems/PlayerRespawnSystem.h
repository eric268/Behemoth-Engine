#pragma once
#include "BehemothEngine.h"

struct PlayerRespawnSystem
{
	void Run(const float deltaTime, ECS::Registry& registry);
};

