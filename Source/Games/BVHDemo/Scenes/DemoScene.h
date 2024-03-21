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

	void Move(float deltaTime, ECS::EntityHandle handle, Behemoth::KeyCode forward, Behemoth::KeyCode back, Behemoth::KeyCode right, Behemoth::KeyCode left);

private:
	ECS::EntityHandle mainCameraHandle;
	ECS::EntityHandle environmentLighting;
	ECS::EntityHandle demoCubes[5];
};
