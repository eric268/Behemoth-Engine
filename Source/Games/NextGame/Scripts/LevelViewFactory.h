#pragma once

#include "BehemothEngine.h"

class LevelViewFactory
{
public:
	static ECS::EntityHandle CreateLevelViewEntity(
		ECS::Registry& registry,
		const BMath::Vector3& spawnLocation,
		float maxZoom,
		float currentZoom,
		float maxHeight,
		float currentHeight);

private:
	static void AddInputBindings(ECS::Registry& registry, const ECS::EntityHandle& handle);
};

