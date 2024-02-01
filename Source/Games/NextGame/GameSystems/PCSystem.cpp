#include "pch.h"
#include "PCSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Components/PhysicsComponents.h"
#include "GameComponents/Player/PCComponent.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/AudioComponents.h"
#include "Physics/ProjectileMotion.h"
#include "Misc/CameraHelper.h"
#include "Math/Quaternion.h"

void PCSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [entity, playerComp, pcComp
	]: registry.Get<PlayerComponent, PCComponent>())
	{
		// Means we are in a different view mode, do don't allow input
		if (!playerComp->isActive)
		{
			continue;
		}

		IncreasePower(deltaTime, playerComp, pcComp);
		DecreasePower(deltaTime, playerComp, pcComp);

		Look(deltaTime, registry, playerComp, pcComp);
		Aim (deltaTime, registry, playerComp, pcComp);
		Fire(registry, entity, playerComp, pcComp);

		RotateMeshWhileMoving(registry, entity, playerComp);
		CheckPlayerLanded(registry, entity, playerComp);

		if (playerComp->canFire && !playerComp->levelComplete)
		{
			SetArrowMeshVisibility(registry, playerComp, true);
		}
	}
}

void PCSystem::IncreasePower(const float deltaTime, PlayerComponent* playerComp, PCComponent* pcComp)
{
	const float triggerInput = Behemoth::Input::GetRightControllerTrigger(0);
	if (triggerInput > 0.0f)
	{
		playerComp->currentPower += playerComp->chargeSpeed * deltaTime * triggerInput;
	}
	else if (Behemoth::Input::IsKeyHeld(pcComp->increasePowerKC))
	{
		playerComp->currentPower += playerComp->chargeSpeed * deltaTime;
	}

	playerComp->currentPower = std::min(100.0f, playerComp->currentPower);
}

void PCSystem::DecreasePower(const float deltaTime, PlayerComponent* playerComp, PCComponent* pcComp)
{
	const float triggerInput = Behemoth::Input::GetLeftControllerTrigger(0);
	if (triggerInput > 0.0f)
	{
		playerComp->currentPower -= playerComp->chargeSpeed * deltaTime * triggerInput;
	}
	else if (Behemoth::Input::IsKeyHeld(pcComp->decreasePowerKC))
	{
		playerComp->currentPower -= playerComp->chargeSpeed * deltaTime;
	}

	playerComp->currentPower = std::max(0.0f, playerComp->currentPower);
}

void PCSystem::Fire(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, PlayerComponent* playerComp, PCComponent* pcComp)
{
	// No charge so skip firing check
	if (playerComp->currentPower <= 0.0f || !playerComp->canFire || playerComp->levelComplete)
	{
		return;
	}

	if (Behemoth::Input::IsKeyDown(pcComp->fireProjectileKC) || Behemoth::Input::IsControllerKeyDown(pcComp->fireProjectileCC))
	{
		Behemoth::MeshComponent* arrowMesh = registry.GetComponent<Behemoth::MeshComponent>(playerComp->arrowMeshHandle);
		if (arrowMesh)
		{
			arrowMesh->isVisible = false;
		}

		Behemoth::TransformComponent* playerTransform = registry.GetComponent<Behemoth::TransformComponent>(playerComp->projectileHandle);

		if (Behemoth::VelocityComponent* velocityComponent = registry.GetComponent<Behemoth::VelocityComponent>(entityHandle))
		{
			velocityComponent->velocity = Behemoth::ProjectileMotion::CalculateInitalVelocity(
				playerComp->currentPower * 0.33f, 
				playerTransform->forwardVector);
		}

		if (Behemoth::RigidBodyComponent* playerRigidBody = registry.GetComponent<Behemoth::RigidBodyComponent>(entityHandle))
		{
			playerRigidBody->affectedByGravity = true;
		}

		SetArrowMeshVisibility(registry, playerComp, false);
		playerComp->canFire = false;
		// Reset charge amount, could keep it previous amount of set it to default like 50
		playerComp->lastLocation = playerTransform->worldPosition;
		playerComp->strokesUsed++;
	}
}

void PCSystem::Look(const float deltaTime, ECS::Registry& registry, PlayerComponent* playerComp, PCComponent* pcComp)
{
	Behemoth::AnalogInput input = Behemoth::Input::GetLeftControllerAnalog();

 	float keyInputX = Behemoth::Input::IsKeyHeld(pcComp->lookLeftKC) - Behemoth::Input::IsKeyHeld(pcComp->lookRightKC);
	float keyInputY = Behemoth::Input::IsKeyHeld(pcComp->lookUpKC)   - Behemoth::Input::IsKeyHeld(pcComp->lookDownKC);

	if (input.x == 0.0f && input.y == 0.0f && keyInputX == 0.0f && keyInputY == 0.0f)
	{
		return;
	}

	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(playerComp->cameraSpringArm))
	{
		BMath::Quaternion quatX = BMath::Quaternion::Identity();
		BMath::Quaternion quatY = BMath::Quaternion::Identity();

		Behemoth::CameraComponent* cameraComponent = Behemoth::CameraHelper::GetMainCamera(registry);
		if (!cameraComponent)
		{
			LOGMESSAGE(Error, "Error finding main camera component");
			return;
		}

		if (input.x != 0.0f)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(-input.x), BMath::Vector3(cameraComponent->upVector));
		}
		else if (keyInputX != 0.0f)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(keyInputX), BMath::Vector3(cameraComponent->upVector));
 		}

		if (input.y != 0.0f)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(-input.y), BMath::Vector3(cameraComponent->rightVector));
		}
		else if (keyInputY != 0.0f)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(keyInputY), BMath::Vector3(cameraComponent->rightVector));
		}

		parentRotationComponent->quat = quatY * quatX;
		parentRotationComponent->isAdditive = true;
	}
}

void PCSystem::Aim(const float deltaTime, ECS::Registry& registry, PlayerComponent* playerComp, PCComponent* pcComp)
{
	Behemoth::AnalogInput input = Behemoth::Input::GetRightControllerAnalog();

 	float keyInputX = Behemoth::Input::IsKeyHeld(pcComp->aimRightKC) - Behemoth::Input::IsKeyHeld(pcComp->aimLeftKC);
 	float keyInputY = Behemoth::Input::IsKeyHeld(pcComp->aimDownKC)  - Behemoth::Input::IsKeyHeld(pcComp->aimUpKC);

	if (input.x == 0.0f && input.y == 0.0f && keyInputX == 0.0f && keyInputY == 0.0f)
	{
		return;
	}

	if (Behemoth::RotationComponent* parentRotationComp = registry.AddComponent<Behemoth::RotationComponent>(playerComp->projectileHandle))
	{
		BMath::Quaternion quatX = BMath::Quaternion::Identity();
		BMath::Quaternion quatY = BMath::Quaternion::Identity();

		Behemoth::TransformComponent* projectileTransform = registry.GetComponent<Behemoth::TransformComponent>(playerComp->projectileHandle);

		if (input.x != 0.0f)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(input.x), projectileTransform->upVector);
		}
		else if (keyInputX)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(keyInputX), projectileTransform->upVector);
 		}

		if (input.y != 0.0f)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(-input.y), projectileTransform->rightVector);
		}
 		else if (keyInputY)
 		{
 			quatY = BMath::Quaternion(DEGREE_TO_RAD(keyInputY), projectileTransform->rightVector);
 		}

		parentRotationComp->quat = (quatX * quatY).Normalize();
		parentRotationComp->isAdditive = true;
	}
}


void PCSystem::SetArrowMeshVisibility(ECS::Registry& registry, PlayerComponent* playerComp, bool isVisible)
{
	Behemoth::MeshComponent* arrowMeshComponent = registry.GetComponent<Behemoth::MeshComponent>(playerComp->arrowMeshHandle);
	if (!arrowMeshComponent)
	{
		LOGMESSAGE(Error, "Unable to get arrow mesh component");
		return;
	}
	
	arrowMeshComponent->isVisible = isVisible;
}

void PCSystem::RotateMeshWhileMoving(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, PlayerComponent* playerComponent)
{
	if (Behemoth::RotationComponent* parentRotationComp = registry.AddComponent<Behemoth::RotationComponent>(playerComponent->playerMeshHandle))
	{
		if (Behemoth::VelocityComponent* velocityComp = registry.GetComponent<Behemoth::VelocityComponent>(entityHandle))
		{
			float speed = velocityComp->velocity.Magnitude();
			if (speed < 1.0f)
			{
				return;
			}

			BMath::Vector3 normalizedVelocity = BMath::Vector3::Normalize(velocityComp->velocity);
			BMath::Vector3 dir = BMath::Vector3::Normalize(BMath::Vector3(-velocityComp->velocity.z, velocityComp->velocity.y, velocityComp->velocity.x));
			parentRotationComp->quat = BMath::Quaternion(DEGREE_TO_RAD(5.0f), dir);
			parentRotationComp->isAdditive = true;
		}
	}
}

void PCSystem::CheckPlayerLanded(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, PlayerComponent* playerComp)
{
	Behemoth::CollisionDataComponent* collisionDataComp = registry.GetComponent<Behemoth::CollisionDataComponent>(entityHandle);
	if (!collisionDataComp)
	{
		// Not colliding with ground so exit 
		return;
	}

	if (Behemoth::VelocityComponent* velocity = registry.GetComponent<Behemoth::VelocityComponent>(entityHandle))
	{
		if (BMath::Vector3::SquaredMagnitude(velocity->velocity) <=  1.0f)
		{
			velocity->velocity = BMath::Vector3(0.0f);
			playerComp->canFire = true;
		}
	}
}