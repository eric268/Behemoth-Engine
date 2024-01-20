#pragma once

#include "BehemothEngine.h"
#include "Scripts/GameObjects/PlayerProjectile.h"

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
	float counter = 0.0f;
	float powerChargeSpeed = 10.0f;

	ECS::EntityHandle obstacleHandle;

	ECS::EntityHandle mainCameraHandle;
	ECS::EntityHandle cameraSpringArm;

	ECS::EntityHandle environmentLighting;
	ECS::EntityHandle pointLight;
	ECS::EntityHandle skySphere;
	ECS::EntityHandle groundEntity;

	PlayerProjectile playerCharacter;
};
            