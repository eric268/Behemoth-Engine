#pragma once
#include "ECS/Registry.h"
#include "ECS/Entity.h"

class LevelHelper
{
public:
	static void CheckOutOfBound(ECS::Registry& registry, const ECS::EntityHandle& playerHandle, const ECS::EntityHandle& oobHandle);

private:

};

