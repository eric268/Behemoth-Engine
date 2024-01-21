#pragma once
#include "BehemothEngine.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "GameComponents/LevelViewComponent.h"

struct ViewModeChange
{
	static bool ChangeViewMode(ECS::Registry& registry, ECS::EntityHandle playerHandle, ECS::EntityHandle levelViewHandle);
};

