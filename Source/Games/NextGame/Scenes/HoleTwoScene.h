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
	void ConstructEnvironment(ECS::Registry& registry) override; 

	ECS::EntityHandle levelViewEntity;
	ECS::EntityHandle parTextEntity;
	ECS::EntityHandle levelCompleteText;

	ECS::EntityHandle mainBarriers[3];
	ECS::EntityHandle barrier1;

	ECS::EntityHandle grassPatch1;
	ECS::EntityHandle grassPatch2;

	ECS::EntityHandle sandPatch2;



	int par;
	float delayUntilSceneChange;
	bool changeScene;


};
