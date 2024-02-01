#pragma once

#include "GameScene.h"

class HoleOneScene : public GameScene
{
public:
	using Super = GameScene;

	HoleOneScene();
	void Initialize() override;
	void Update(const float deltaTime) override;
	void OnEvent(Behemoth::Event& e) override;
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

private:
	void ConstructEnvironment(ECS::Registry& registry) override;

	ECS::EntityHandle goalViewHandle;
	ECS::EntityHandle grassPlatformHandle;
	ECS::EntityHandle sandTrap1Handle;
	ECS::EntityHandle sandTrap2Handle;
	ECS::EntityHandle obstacleHandle;
	ECS::EntityHandle parTextHandle;
};
            