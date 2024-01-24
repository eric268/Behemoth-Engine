#pragma once
#include "BehemothEngine.h"

class PlayerFallSystem
{
public:
	void Run(const float deltaTime, ECS::Registry& registry);
	
private:
	void RespawnPlayer(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);
};

