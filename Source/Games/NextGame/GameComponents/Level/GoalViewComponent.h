#pragma once
#include "ECS/Component.h"
#include "ECS/Entity.h"

struct GoalViewComponent : public ECS::Component
{
	GoalViewComponent() = default;

	GoalViewComponent(
		bool isActive,
		float movementSpeed,
		float minZoomDistance,
		float maxZoomDistance,
		float currentZoomCounter,
		float maxVertical,
		float currentVerticalDistance) 
		:
		isActive(isActive),
		cameraMoveSpeed(movementSpeed),
		minZoomDistance(minZoomDistance),
		maxZoomDistance(maxZoomDistance),
		currentZoomCounter(currentZoomCounter),
		maxVerticalDistance(maxVertical),
		currentVerticalDistance(currentVerticalDistance)
	{}

	ECS::EntityHandle attachedCamera;
	bool isActive;
	float cameraMoveSpeed;

	float minZoomDistance;
	float maxZoomDistance;
	float currentZoomCounter;

	float maxVerticalDistance;
	float currentVerticalDistance;
};