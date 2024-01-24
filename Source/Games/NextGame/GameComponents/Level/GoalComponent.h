#pragma once
#include "ECS/Component.h"
#include "ECS/Entity.h"

#include <vector>

struct GoalComponent : public ECS::Component
{
	std::vector<ECS::EntityHandle> goalColliderHandles;
};