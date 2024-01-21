#pragma once
#include "BehemothEngine.h"

struct GoalObject
{
	static ECS::EntityHandle CreateGoal(ECS::Registry& registry, const BMath::Vector3& location, const BMath::Vector3& scale, float rotationAngle);
};

