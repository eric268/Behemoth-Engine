#pragma once
#include "ECS/Component.h"
#include "Math/Vector.h"

struct MovingObsComponent : public ECS::Component
{
	MovingObsComponent() = default;
	MovingObsComponent(
		BMath::Vector3 axis,
		float speed,
		float maxDistance,
		float counter,
		float dir = 1.0f,
		bool isActive = true) : 
		movementAxis(axis), 
		movementSpeed(speed), 
		maxDistance(maxDistance),
		movementCounter(counter), 
		isActive(isActive),
		dir(dir)
	{}
	float dir;
	bool isActive;
	BMath::Vector3 movementAxis;
	float movementSpeed;
	float movementCounter;
	float maxDistance;
};