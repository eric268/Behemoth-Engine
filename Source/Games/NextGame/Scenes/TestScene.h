#pragma once

#include "BehemothEngine.h"

class TestScene : public Behemoth::Scene
{
public:
	using Super = Behemoth::Scene;

	TestScene();
	void Initalize() override;
	void OnEvent(Behemoth::Event& e);
	void Update(const float deltaTime) override;
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

private:
	ECS::EntityHandle mainCameraHandle;
	ECS::EntityHandle environmentLighting;
	ECS::EntityHandle pointLight;
	ECS::EntityHandle skySphere;

	ECS::EntityHandle exampleParentEntity;
	ECS::EntityHandle exampleChildEntity1;
	ECS::EntityHandle exampleChildEntity2;

};
