#pragma once
#include "GameScene.h"

class MainMenuScene : public GameScene
{
public:
	using Super = GameScene;

	MainMenuScene();
	void Initialize() override;
	void InitializeSystems();
	void Update(const float deltaTime) override;
	void OnEvent(Behemoth::Event& e) override;
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

protected:
	void ConstructEnvironment(ECS::Registry& registry) override;

private:
	void CreateInstructionsText(ECS::Registry& registry);

	ECS::EntityHandle teeOffPlatformHandle;
	ECS::EntityHandle titleTextHandle;
	ECS::EntityHandle controllerInstructionsHandles[6];
	ECS::EntityHandle keyboardInstructionsHandles[6];
};

