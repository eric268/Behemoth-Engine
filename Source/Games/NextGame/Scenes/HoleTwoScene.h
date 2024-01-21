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
	void ConstructEnvironment(ECS::Registry& registry); 

	ECS::EntityHandle levelViewEntity;
	ECS::EntityHandle parTextEntity;
	ECS::EntityHandle levelCompleteText;

	ECS::EntityHandle mainBarriers[3];

	ECS::EntityHandle grassPatch1;
	ECS::EntityHandle grassPatch2;

	ECS::EntityHandle pointLight1;
	ECS::EntityHandle pointLight2;
	ECS::EntityHandle pointLight3;
	ECS::EntityHandle pointLight4;
	ECS::EntityHandle pointLight5;

	int par;
	float delayUntilSceneChange;
	bool changeScene;


};
