#pragma once
#include "BehemothEngine.h"

struct PlayerComponent : public ECS::Component
{
	PlayerComponent() = default;

	PlayerComponent(
		const ECS::EntityHandle& camera,
		const ECS::EntityHandle& springArm,
		const ECS::EntityHandle& proj,
		const ECS::EntityHandle& mesh,
		const ECS::EntityHandle& arrowMesh,
		float chargeSpeed)
		:
		attachedCamera(camera),
		cameraSpringArm(springArm),
		projectileHandle(proj),
		playerMeshHandle(mesh),
		arrowMeshHandle(arrowMesh),
		chargeSpeed(chargeSpeed),
		currentPower(0.0f),
		canFire(true)
	{}

	ECS::EntityHandle attachedCamera;
	ECS::EntityHandle cameraSpringArm;

	ECS::EntityHandle projectileHandle;
	ECS::EntityHandle playerMeshHandle;
	ECS::EntityHandle arrowMeshHandle;

	float currentPower;
	float chargeSpeed;
	bool canFire;
};

