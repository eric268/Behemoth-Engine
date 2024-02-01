#pragma once

#include "BehemothEngine.h"

struct PlayerFactory
{
public:
	static ECS::EntityHandle CreatePlayer(ECS::Registry& registry, const BMath::Vector3& spawnLocation);

private:
	static void AddInputBindings(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);
	static void InitializeHUD(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);
};

