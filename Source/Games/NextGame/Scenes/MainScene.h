#pragma once

#include "BehemothEngine.h"
#include "Physics/BVH.h"

class MainScene : public Behemoth::Scene
{
public:
	using Super = Behemoth::Scene;

	MainScene();
	void OnEvent(Behemoth::Event& e) override;
	void Initalize() override;
	void Update(const float deltaTime) override;
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

	ECS::EntityHandle pointLight;
	ECS::EntityHandle cubes[8];
	ECS::EntityHandle playerHandle;

	bool doOnce = true;
	int counter = 0;

private:
	void InitSystems();
};
            