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
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

private:
	void ConstructEnvironment(ECS::Registry& registry);

	ECS::EntityHandle goalViewHandle;
	ECS::EntityHandle parTextHandle;
	ECS::EntityHandle rockPlatformHandle;
	ECS::EntityHandle barrierHandle;
};


