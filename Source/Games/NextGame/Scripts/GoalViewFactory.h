#pragma once

#include "BehemothEngine.h"

class GoalViewFactory
{
public:
	static ECS::EntityHandle CreateGoalViewEntity(
		ECS::Registry& registry,
		const BMath::Vector3& spawnLocation,
		float currentZoom,
		float currentHeight);

private:
	static void AddInputBindings(ECS::Registry& registry, const ECS::EntityHandle& handle);
};

