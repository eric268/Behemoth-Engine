#pragma once

#include "BehemothEngine.h"

struct ViewMode
{
	static bool ToggleViewMode(ECS::Registry& registry, ECS::EntityHandle playerHandle, ECS::EntityHandle levelViewHandle);
};

