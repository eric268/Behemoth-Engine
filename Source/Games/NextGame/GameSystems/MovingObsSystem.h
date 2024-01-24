#pragma once
#include "BehemothEngine.h"

struct MovingObsSystem
{
	void Run(const float deltaTime, ECS::Registry& registry);
};

