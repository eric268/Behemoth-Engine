#include "pch.h"
#include "CameraControllerSystem.h"
#include "GameComponents/CameraControllerComponent.h"
#include "Components/Components.h"


void CameraControllerSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	auto components = registry.Get<Behemoth::TransformComponent, Behemoth::VelocityComponent, Behemoth::RotationComponent, CameraControllerComponent>();

	for (auto& [entity, transformComp, velocityComp, rotationComp, controllerComp] : components)
	{
		// rotationComp->speed = 0;

		BMath::Vector3 velocity = BMath::Vector3::Zero();

		if (Behemoth::Input::IsKeyHeld(controllerComp->forward))
		{
			// velocity += transformComp->forwardVector;
			 velocity += BMath::Vector3::Forward();
			
		}

		if (Behemoth::Input::IsKeyHeld(controllerComp->back))
		{
			// velocity -= transformComp->forwardVector;
			velocity -= BMath::Vector3::Forward();
		}

		if (Behemoth::Input::IsKeyHeld(controllerComp->right))
		{
			// velocity += transformComp->rightVector;
			velocity += BMath::Vector3::Right();
		}

		if (Behemoth::Input::IsKeyHeld(controllerComp->left))
		{
			// velocity -= transformComp->rightVector;
			velocity -= BMath::Vector3::Right();
		}

		if (Behemoth::Input::IsKeyHeld(controllerComp->up))
		{
			// velocity += transformComp->upVector;
			velocity += BMath::Vector3::Up();
		}
		if (Behemoth::Input::IsKeyHeld(controllerComp->down))
		{
			// velocity -= transformComp->upVector;
			velocity -= BMath::Vector3::Up();
		}

// 		if (controllerComp->enableRotation)
// 		{
// 			if (Behemoth::Input::IsKeyHeld(Behemoth::KC_I))
// 			{
// 				rotationComp->speed = 1.0f;
// 				rotationComp->axis = Behemoth::RotationComponent::X_AXIS;
// 			}
// 
// 			if (Behemoth::Input::IsKeyHeld(Behemoth::KC_K))
// 			{
// 				rotationComp->speed = -1.0f;
// 				rotationComp->axis = Behemoth::RotationComponent::X_AXIS;
// 			}
// 
// 			if (Behemoth::Input::IsKeyHeld(Behemoth::KC_J))
// 			{
// 				rotationComp->speed = -1.0f;
// 				rotationComp->axis = Behemoth::RotationComponent::Y_AXIS;
// 			}
// 			if (Behemoth::Input::IsKeyHeld(Behemoth::KC_L))
// 			{
// 				rotationComp->speed = 1.0f;
// 				rotationComp->axis = Behemoth::RotationComponent::Y_AXIS;
// 			}
// 		}

		velocity.Normalize();
		velocity *= controllerComp->movementSpeed;

		velocityComp->velocity = velocity;
	}
}