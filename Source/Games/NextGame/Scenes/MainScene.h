#pragma once
            
#include "BehemothCore.h"
            
class MainScene : public Behemoth::Scene
{
public:
	MainScene();
	void OnEvent(Behemoth::Event& e) override;
	void Init() override;
	void Update(const float deltaTime) override;
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

	ECS::EntityHandle pointLight;
	ECS::EntityHandle cube1;
	ECS::EntityHandle cube2;

	Behemoth::Collision::BVH collisionBVH;

	bool doOnce = true;
	int counter = 0;

private:
	void InitSystems();
};
            