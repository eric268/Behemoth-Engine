#pragma once

#include "BehemothEngine.h"


class MainScene : public Behemoth::Scene
{
public:
	using Super = Behemoth::Scene;

	MainScene();
	void Initalize() override;
	void ProcessEvent(Behemoth::Event& e) override;
	void Update(const float deltaTime) override;
	void InitalizeSystems();
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

private:

	bool levelViewActive = false;

	ECS::EntityHandle levelViewEntity;

	ECS::EntityHandle obstacleHandle;

	ECS::EntityHandle bottomOOBTrigger;

	ECS::EntityHandle goalObject;

	ECS::EntityHandle environmentLighting;
	ECS::EntityHandle pointLight;
	ECS::EntityHandle skySphere;
	ECS::EntityHandle groundEntity;

	ECS::EntityHandle parEntity;

	ECS::EntityHandle playerCharacter;
	ECS::EntityHandle teeOffPlatform;
};
            