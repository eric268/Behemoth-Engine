#pragma once

#include "BehemothEngine.h"


class MainScene : public Behemoth::Scene
{
public:
	using Super = Behemoth::Scene;

	MainScene();
	void Initalize() override;
	void Update(const float deltaTime) override;
	void OnEvent(Behemoth::Event& e) override;
	void InitalizeSystems();
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

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
};
            