#pragma once

#include "GameScene.h"

class PlayerComponent;

class MainMenuScene : public GameScene
{
public:
	using Super = Behemoth::Scene;

	MainMenuScene();
	void Initalize() override;
	void Update(const float deltaTime) override;
	void OnEvent(Behemoth::Event& e) override;
	void InitalizeSystems();
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

protected:
	void ConstructEnvironment(ECS::Registry& registry) override;

private:
	ECS::EntityHandle teeOffPlatform;
	ECS::EntityHandle levelViewEntity;
	ECS::EntityHandle titleTextEntity;


	float delayUntilSceneChange;
	bool changeScene;
};

