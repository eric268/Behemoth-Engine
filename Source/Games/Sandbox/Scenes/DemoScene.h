#pragma once

#include "BehemothEngine.h"

class DemoScene : public Behemoth::Scene
{
public:
	using Super = Behemoth::Scene;

	DemoScene();
	void Initialize() override;
	void OnEvent(Behemoth::Event& e) override;
	void Update(const float deltaTime) override;
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

private:
	ECS::EntityHandle mainCameraHandle;
	ECS::EntityHandle environmentLighting;
	ECS::EntityHandle skySphereHandle;

	ECS::EntityHandle parentHandle;
	ECS::EntityHandle childHandle1;
	ECS::EntityHandle childHandle2;
};
