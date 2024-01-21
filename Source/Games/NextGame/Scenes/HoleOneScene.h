#pragma once


#include "GameScene.h"

class HoleOneScene : public GameScene
{
public:
	using Super = Behemoth::Scene;

	HoleOneScene();
	void Initalize() override;
	void Update(const float deltaTime) override;
	void OnEvent(Behemoth::Event& e) override;
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

private:
	ECS::EntityHandle levelViewEntity;

	ECS::EntityHandle grassEntity;
	ECS::EntityHandle sandTrap1;
	ECS::EntityHandle sandTrap2;
	ECS::EntityHandle obstacleHandle;
	ECS::EntityHandle parTextEntity;

	int par;
	bool changeScene;
};
            