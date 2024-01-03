#include "pch.h"
#include "CameraControllerSystem.h"
#include "GameComponents/CameraControllerComponent.h"


void CameraControllerSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	auto components = registry.Get<Behemoth::TransformComponent, Behemoth::VelocityComponent, Behemoth::RotationComponent, CameraControllerComponent>();

	for (auto& [entity, transformComp, velocityComp, rotationComp, controllerComp] : components)
	{
		velocityComp->velocity = Math::Vector3::Zero();
		rotationComp->speed = 0;

		if (Behemoth::Input::IsKeyHeld(controllerComp->forward))
		{
			velocityComp->velocity += transformComp->forwardVector;
		}

		if (Behemoth::Input::IsKeyHeld(controllerComp->back))
		{
			velocityComp->velocity -= transformComp->forwardVector;
		}

		if (Behemoth::Input::IsKeyHeld(controllerComp->right))
		{
			velocityComp->velocity -= transformComp->rightVector;
		}

		if (Behemoth::Input::IsKeyHeld(controllerComp->left))
		{
			velocityComp->velocity += transformComp->rightVector;
		}

		if (controllerComp->enableRotation)
		{
			if (Behemoth::Input::IsKeyHeld(Behemoth::KC_I))
			{
				rotationComp->speed = 1.0f;
				rotationComp->axis = Behemoth::RotationComponent::X_AXIS;
			}

			if (Behemoth::Input::IsKeyHeld(Behemoth::KC_K))
			{
				rotationComp->speed = -1.0f;
				rotationComp->axis = Behemoth::RotationComponent::X_AXIS;
			}

			if (Behemoth::Input::IsKeyHeld(Behemoth::KC_J))
			{
				rotationComp->speed = -1.0f;
				rotationComp->axis = Behemoth::RotationComponent::Y_AXIS;
			}
			if (Behemoth::Input::IsKeyHeld(Behemoth::KC_L))
			{
				rotationComp->speed = 1.0f;
				rotationComp->axis = Behemoth::RotationComponent::Y_AXIS;
			}
		}


		velocityComp->velocity.Normalize();
		velocityComp->velocity *= 0.75f;
	}
}