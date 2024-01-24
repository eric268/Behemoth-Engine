#pragma once

#include "BehemothEngine.h"

class DebugScene : public Behemoth::Scene
{
public:
	using Super = Behemoth::Scene;

	DebugScene();
	void Initialize() override;
	void OnEvent(Behemoth::Event& e) override;
	void Update(const float deltaTime) override;
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

	void MoveObject(ECS::EntityHandle handle, Behemoth::KeyCode forward, Behemoth::KeyCode back, Behemoth::KeyCode left, Behemoth::KeyCode right, Behemoth::KeyCode up, Behemoth::KeyCode down, Behemoth::KeyCode rotLeft, Behemoth::KeyCode rotRight);

private:
	ECS::EntityHandle mainCameraHandle;
	ECS::EntityHandle environmentLighting;
	ECS::EntityHandle pointLight;
	ECS::EntityHandle skySphere;

	ECS::EntityHandle exampleParentEntity;
	ECS::EntityHandle exampleChildEntity1;
	ECS::EntityHandle exampleChildEntity2;
};
