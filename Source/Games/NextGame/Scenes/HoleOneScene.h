#pragma once

#include "BehemothEngine.h"


class HoleOneScene : public Behemoth::Scene
{
public:
	using Super = Behemoth::Scene;

	HoleOneScene();
	void Initalize() override;
	void Update(const float deltaTime) override;
	void OnEvent(Behemoth::Event& e) override;
	void InitalizeSystems();
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }
	void NextHole();

private:
	ECS::EntityHandle environmentLighting;
	ECS::EntityHandle pointLight;
	ECS::EntityHandle skySphere;
	ECS::EntityHandle bottomOOBTrigger;

	ECS::EntityHandle goalObject;
	ECS::EntityHandle playerCharacter;
	ECS::EntityHandle levelViewEntity;

	ECS::EntityHandle teeOffPlatform;
	ECS::EntityHandle grassEntity;
	ECS::EntityHandle sandTrap1;
	ECS::EntityHandle sandTrap2;
	ECS::EntityHandle obstacleHandle;

	ECS::EntityHandle parTextEntity;
	ECS::EntityHandle levelCompleteText;

	int par;
	float delayUntilSceneChange;
	bool changeScene;
};
            