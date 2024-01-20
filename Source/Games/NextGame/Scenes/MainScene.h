#pragma once

#include "BehemothEngine.h"

class MainScene : public Behemoth::Scene
{
public:
	using Super = Behemoth::Scene;

	MainScene();
	void Initalize() override;
	void ProcessEvent(Behemoth::Event& e) override;
	void Update(const float deltaTime) override;
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

private:
	int counter = 0;
	ECS::EntityHandle mainCameraHandle;
	ECS::EntityHandle cameraSpringArm;

	ECS::EntityHandle environmentLighting;
	ECS::EntityHandle pointLight;
	ECS::EntityHandle skySphere;

	ECS::EntityHandle exampleParentEntity;
	ECS::EntityHandle exampleChildEntity1;
	ECS::EntityHandle exampleChildEntity2;

	ECS::EntityHandle projectileEntity;
	ECS::EntityHandle arrowIconEntity;

	ECS::EntityHandle playerEntity;

	ECS::EntityHandle groundEntity;
};
            