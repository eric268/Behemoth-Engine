#pragma once

#include "GameScene.h"

class HoleThreeScene : public GameScene
{
public:
	using Super = GameScene;

	HoleThreeScene();
	void Initialize() override;
	void Update(const float deltaTime) override;
	void OnEvent(Behemoth::Event& e) override;
	void InitializeSystems();
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

private:
	void ConstructEnvironment(ECS::Registry& registry);
	ECS::EntityHandle levelViewEntity;
	ECS::EntityHandle parTextEntity;
	ECS::EntityHandle levelCompleteText;
	ECS::EntityHandle rockPlatform1;
	ECS::EntityHandle barrier1;
};


