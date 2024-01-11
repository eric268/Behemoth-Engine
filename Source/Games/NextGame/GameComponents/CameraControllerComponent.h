#pragma once
#include "ECS/Component.h"
#include "Input/InputCodes.h"

struct CameraControllerComponent : ECS::Component
{
 	CameraControllerComponent() : 
        movementSpeed           (0.0f), 
        rotationSpeed           (0.0f), 
        enableRotation          (false), 
        forward                 (Behemoth::KeyCode::KC_None), 
        back                    (Behemoth::KeyCode::KC_None), 
        left                    (Behemoth::KeyCode::KC_None), 
        right                   (Behemoth::KeyCode::KC_None), 
        up                      (Behemoth::KeyCode::KC_None), 
        down                    (Behemoth::KeyCode::KC_None) 
    {}

 	CameraControllerComponent(
        float moveSpeed, 
        float rotSpeed, 
        bool enableRot, 
        Behemoth::KeyCode f, 
        Behemoth::KeyCode b, 
        Behemoth::KeyCode l, 
        Behemoth::KeyCode r, 
        Behemoth::KeyCode u, 
        Behemoth::KeyCode d) 
        : 
        movementSpeed(moveSpeed), 
        rotationSpeed(rotSpeed), 
        enableRotation(enableRot), 
        forward(f), 
        back(b), 
        left(l), 
        right(r), 
        up(u), 
        down(d) 
    {}

 	float movementSpeed;
 	float rotationSpeed;
    bool enableRotation;

    Behemoth::KeyCode forward;
    Behemoth::KeyCode back;
    Behemoth::KeyCode right;
    Behemoth::KeyCode left;
    Behemoth::KeyCode up;
    Behemoth::KeyCode down;

};