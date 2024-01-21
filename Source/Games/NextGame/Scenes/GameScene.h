#pragma once
#include "World/Scene.h"
#include "BehemothEngine.h"

class GameScene : public Behemoth::Scene
{
public:
	void CheckOutOfBound(ECS::Registry& registry, const ECS::EntityHandle& playerHandle, const ECS::EntityHandle& oobHandle);

protected:
	ECS::EntityHandle environmentLighting;
	ECS::EntityHandle skySphere;
	ECS::EntityHandle bottomOOBTrigger;
	ECS::EntityHandle playerCharacter;
	ECS::EntityHandle teeOffPlatform;
};

