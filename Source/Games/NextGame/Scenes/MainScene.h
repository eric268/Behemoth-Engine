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

	ECS::Entity pointLight;
	ECS::Entity cube1;
	ECS::Entity cube2;

private:
	void InitSystems();
};
            