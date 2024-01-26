#pragma once
#include "BehemothEngine.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "GameComponents/Level/LevelViewComponent.h"

struct ViewMode
{
	static bool ToggleViewMode(ECS::Registry& registry, ECS::EntityHandle playerHandle, ECS::EntityHandle levelViewHandle);
};

