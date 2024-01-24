#pragma once
#include "GameScene.h"

class MainMenuScene : public GameScene
{
public:
	using Super = Behemoth::Scene;

	MainMenuScene();
	void Initialize() override;
	void Update(const float deltaTime) override;
	void OnEvent(Behemoth::Event& e) override;
	void InitializeSystems();
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

protected:
	void ConstructEnvironment(ECS::Registry& registry) override;

private:
	ECS::EntityHandle teeOffPlatform;
	ECS::EntityHandle levelViewEntity;
	ECS::EntityHandle titleTextEntity;

	ECS::EntityHandle controllerInstructions[6];
	ECS::EntityHandle keyboardInstructions[6];

	void CreateInstructionsText(ECS::Registry& registry);

	float delayUntilSceneChange;
	bool changeScene;
};

