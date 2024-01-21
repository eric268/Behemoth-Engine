#pragma once
#include "BehemothEngine.h"

struct PlayerHUDSystem
{
public:
	void Run(const float deltaTime, ECS::Registry& registry);
};

