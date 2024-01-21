#pragma once

#include "GameScene.h"

class HoleTwoScene : public GameScene

{
public:
	using Super = Behemoth::Scene;

	HoleTwoScene();
	void Initalize() override;
	void OnEvent(Behemoth::Event& e);
	void Update(const float deltaTime) override;
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

private:

	ECS::EntityHandle goalObject;
	ECS::EntityHandle levelViewEntity;

	ECS::EntityHandle parTextEntity;
	ECS::EntityHandle levelCompleteText;

	int par;
	float delayUntilSceneChange;
	bool changeScene;

};
