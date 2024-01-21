#pragma once
#include "World/Scene.h"
#include "BehemothEngine.h"

#include <unordered_map>

class GameScene : public Behemoth::Scene
{
public:
	void CheckOutOfBound(ECS::Registry& registry, const ECS::EntityHandle& playerHandle, const ECS::EntityHandle& oobHandle);
	std::string GetHoleResultText(int numStrokes, int par);
	bool CheckLevelComplete(ECS::Registry& registry, ECS::EntityHandle& playerHandle);
	void OnHoleComplete(ECS::Registry& registry, ECS::EntityHandle& playerHandle, int par);

protected:
	ECS::EntityHandle environmentLighting;
	ECS::EntityHandle skySphere;
	ECS::EntityHandle bottomOOBTrigger;
	ECS::EntityHandle playerCharacter;
	ECS::EntityHandle teeOffPlatform;
	ECS::EntityHandle levelCompleteText;
	ECS::EntityHandle goalObject;

	float delayUntilSceneChange;

	static std::unordered_map<int, std::string> holeResultsText;
};

