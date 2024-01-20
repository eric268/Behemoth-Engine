#pragma once
#include "BehemothEngine.h"

struct PlayerProjectile
{
public:
	ECS::EntityHandle InitalizePlayer(ECS::Registry& registry);

	ECS::EntityHandle playerHandle;
	ECS::EntityHandle projectileHandle;
	ECS::EntityHandle playerMeshHandle;
	ECS::EntityHandle arrowMeshHandle;
};

