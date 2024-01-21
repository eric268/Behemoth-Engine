#pragma once
#include "ECS/Component.h"
#include "ECS/Entity.h"

struct LevelViewComponent : public ECS::Component
{
	LevelViewComponent() = default;

	LevelViewComponent(
		bool isActive,
		float movementSpeed,
		float maxZoomDistance,
		float currentZoomCounter,
		float maxVertical,
		float currentVerticalDistance) 
		:
		isActive									(isActive),
		cameraMoveSpeed								(movementSpeed),
		maxZoomDistance								(maxZoomDistance),
		currentZoomCounter							(currentZoomCounter),
		maxVerticalDistance							(maxVertical),
		currentVerticalDistance						(currentVerticalDistance)
	{}

	ECS::EntityHandle attachedCamera;
	bool isActive;
	float cameraMoveSpeed;

	float maxZoomDistance;
	float currentZoomCounter;

	float maxVerticalDistance;
	float currentVerticalDistance;
};