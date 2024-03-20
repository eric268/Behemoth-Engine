#pragma once

#include "GameScene.h"

class HoleTwoScene : public GameScene

{
public:
	using Super = GameScene;

	HoleTwoScene();
	void Initialize() override;
	void Update(const float deltaTime) override;
	void OnEvent(Behemoth::Event& e);
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

private:
	void ConstructEnvironment(ECS::Registry& registry) override; 

	ECS::EntityHandle goalViewHandle;
	ECS::EntityHandle parTextHandle;
	ECS::EntityHandle mainBarrierHandles[3];
	ECS::EntityHandle barrierHandle;
	ECS::EntityHandle leftLongGrassPatchHandles[3];
	ECS::EntityHandle grassPatch2Handle;
	ECS::EntityHandle sandPatch2Handle;
};
