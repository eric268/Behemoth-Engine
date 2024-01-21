#pragma once
#include "BehemothEngine.h"

struct PlayerComponent : public ECS::Component
{
	PlayerComponent() = default;

	PlayerComponent(
		bool isActive,
		const ECS::EntityHandle& camera,
		const ECS::EntityHandle& springArm,
		const ECS::EntityHandle& proj,
		const ECS::EntityHandle& mesh,
		const ECS::EntityHandle& arrowMesh,
		float chargeSpeed,
		const BMath::Vector3& lastLoc)
		:
		isActive									(isActive),
		attachedCamera								(camera),
		cameraSpringArm								(springArm),
		projectileHandle							(proj),
		playerMeshHandle							(mesh),
		arrowMeshHandle								(arrowMesh),
		chargeSpeed									(chargeSpeed),
		currentPower								(0.0f),
		canFire										(true),
		lastLocation								(lastLoc),
		strokesUsed									(0),
		levelComplete								(false),
		isRespawning								(false)
	{}

	ECS::EntityHandle attachedCamera;
	ECS::EntityHandle cameraSpringArm;

	ECS::EntityHandle projectileHandle;
	ECS::EntityHandle playerMeshHandle;
	ECS::EntityHandle arrowMeshHandle;

	ECS::EntityHandle hudHandle;

	bool isActive;
	bool isRespawning;
	bool levelComplete;
	int strokesUsed;
	float currentPower;
	float chargeSpeed;
	bool canFire;
	BMath::Vector3 lastLocation;
};

struct PlayerFellComponent : public ECS::Component
{
	PlayerFellComponent() : timeUntilRespawn(0.0f) {}
	PlayerFellComponent(float timeTillRespawn) : timeUntilRespawn(timeTillRespawn) {}
	float timeUntilRespawn;
};

struct PlayerRespawnComponent : public ECS::Component
{
	PlayerRespawnComponent() = default;
};

struct PlayerHUDComponent : public ECS::Component
{
	PlayerHUDComponent() {}
	ECS::EntityHandle strokesUIHandle;
	ECS::EntityHandle powerUIHandle;
	ECS::EntityHandle owningPlayerHandle;
};