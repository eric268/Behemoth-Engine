#pragma once
#include "ECS/Component.h"

struct CameraControllerComponent : ECS::Component
{
 	CameraControllerComponent() : movementSpeed(0.0f), rotationSpeed(0.0f) {}
 	CameraControllerComponent(float moveSpeed, float rotSpeed) : movementSpeed(moveSpeed), rotationSpeed(rotSpeed)  {}
 	float movementSpeed;
 	float rotationSpeed;
};