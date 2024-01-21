#pragma once
#include "BehemothEngine.h"

struct BarrierObject
{
public:
	static ECS::EntityHandle CreateObstacle(
		ECS::Registry& registry,
		const BMath::Vector3& location,
		const BMath::Vector3& scale,
		const BMath::Quaternion& q,
		bool isStatic = true);

};

