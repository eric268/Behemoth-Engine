#include "pch.h"
#include "CameraControllerSystem.h"


void CameraControllerSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	ECS::Entity cameraEntity = Behemoth::CameraHelper::GetMainCameraEntity(registry);

	if (cameraEntity.GetIdentifier() == NULL_ENTITY)
	{
		LOG_ERROR("Unable to find main camera");
		return;
	}
	auto components = registry.GetMultipleComponents<Behemoth::TransformComponent, Behemoth::VelocityComponent, Behemoth::RotationComponent>(cameraEntity);

	auto& [transformComp, velocityComp, rotationComp] = components;
	{
		velocityComp->velocity = Math::Vector3::Zero();
		rotationComp->speed = 0;

		if (Behemoth::Input::IsKeyHeld(Behemoth::KeyCode::KC_Up))
		{
			velocityComp->velocity += transformComp->forwardVector;
		}

		if (Behemoth::Input::IsKeyHeld(Behemoth::KeyCode::KC_Down))
		{
			velocityComp->velocity -= transformComp->forwardVector;
		}

		if (Behemoth::Input::IsKeyHeld(Behemoth::KeyCode::KC_Right))
		{
			velocityComp->velocity += transformComp->rightVector;
		}

		if (Behemoth::Input::IsKeyHeld(Behemoth::KeyCode::KC_Left))
		{
			velocityComp->velocity -= transformComp->rightVector;
		}

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

		velocityComp->velocity.Normalize();
		velocityComp->velocity *= 0.75f;
	}
}