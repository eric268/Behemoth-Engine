#pragma once
#include "ECS/Component.h"
#include "Input/InputCodes.h"

struct CameraControllerComponent : ECS::Component
{
 	CameraControllerComponent() : movementSpeed(0.0f), rotationSpeed(0.0f), forward(Behemoth::KeyCode::KC_None), back(Behemoth::KeyCode::KC_None), left(Behemoth::KeyCode::KC_None), right(Behemoth::KeyCode::KC_None) {}
 	CameraControllerComponent(float moveSpeed, float rotSpeed, Behemoth::KeyCode f, Behemoth::KeyCode b, Behemoth::KeyCode r, Behemoth::KeyCode l) : movementSpeed(moveSpeed), rotationSpeed(rotSpeed), forward(f), back(b), left(l), right(r) {}
 	float movementSpeed;
 	float rotationSpeed;

    Behemoth::KeyCode forward;
    Behemoth::KeyCode back;
    Behemoth::KeyCode right;
    Behemoth::KeyCode left;

};