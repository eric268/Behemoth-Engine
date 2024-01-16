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

	ECS::EntityHandle mainCameraHandle;
	ECS::EntityHandle pointLight;
	ECS::EntityHandle cubes[8];
	ECS::EntityHandle playerHandle;
	ECS::EntityHandle DEBUG_audioEntity;

private:
	void InitSystems();
};
            