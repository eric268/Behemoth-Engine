#include "CameraControllerSystem.h"
#include "Misc/CameraHelper.h"
#include "Components/Components.h"


void CameraControllerSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	ECS::Entity cameraEntity = Behemoth::CameraHelper::GetMainCameraEntity(registry);

	if (cameraEntity.GetIdentifier() != NULL_ENTITY)
	{
		Behemoth::VelocityComponent* velocityComponent = registry.GetComponent<Behemoth::VelocityComponent>(cameraEntity);

		auto [vel, transformComponent] = registry.GetMultipleComponents<Behemoth::VelocityComponent, Behemoth::TransformComponent>(cameraEntity);
		Behemoth::KeyCode keyCode = Behemoth::KeyCode::B_1;// keyEvent.GetKeyCode();

		if (velocityComponent && transformComponent)
		{
			float movementSpeed = 1.0f;
			velocityComponent->velocity = Math::Vector3::Zero();

			if (keyCode == Behemoth::KeyCode::B_W)
			{
				velocityComponent->velocity += transformComponent->forwardVector;
			}
			if (keyCode == Behemoth::KeyCode::B_A)
			{
				velocityComponent->velocity -= transformComponent->rightVector;
			}
			if (keyCode == Behemoth::KeyCode::B_S)
			{
				velocityComponent->velocity -= transformComponent->forwardVector;
			}
			if (keyCode == Behemoth::KeyCode::B_D)
			{
				velocityComponent->velocity += transformComponent->rightVector;
			}

			velocityComponent->velocity.Normalize();
			velocityComponent->velocity *= movementSpeed;
		}

		if (keyCode == Behemoth::KeyCode::B_E)
		{
			Behemoth::RotationComponent* rotationComponent = registry.GetComponent<Behemoth::RotationComponent>(cameraEntity);
			if (rotationComponent)
			{
				rotationComponent->axis = Behemoth::RotationComponent::Y_AXIS;
				rotationComponent->speed = 0.5f;
			}
		}

		if (keyCode == Behemoth::KeyCode::B_Q)
		{
			Behemoth::RotationComponent* rotationComponent = registry.GetComponent<Behemoth::RotationComponent>(cameraEntity);
			if (rotationComponent)
			{
				rotationComponent->axis = Behemoth::RotationComponent::Y_AXIS;
				rotationComponent->speed = -0.5f;
			}
		}
	}
}