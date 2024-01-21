#include "pch.h"
#include "PCSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Components/PhysicsComponents.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Physics/ProjectileMotion.h"
#include "Math/Quaternion.h"

using namespace Behemoth;

void PCSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [entity, playerComponent, pcComponent]: registry.Get<PlayerComponent, PCComponent>())
	{
		// Means we are in a different view mode, do dont allow input
		if (!playerComponent->isActive)
		{
			continue;
		}

		IncreasePower(deltaTime, playerComponent, pcComponent);
		DecreasePower(deltaTime, playerComponent, pcComponent);

		Look(deltaTime, registry, playerComponent, pcComponent);
		Aim(deltaTime, registry, playerComponent, pcComponent);
		Fire(registry, entity, playerComponent, pcComponent);

		RotateMeshWhileMoving(registry, entity, playerComponent);
		CheckPlayerLanded(registry, entity, playerComponent);

		if (playerComponent->canFire && !playerComponent->levelComplete)
		{
			SetArrowMeshVisibility(registry, playerComponent, true);
		}
	}
}

void PCSystem::IncreasePower(const float deltaTime, PlayerComponent* playerComponent, PCComponent* pcComponent)
{
	const float triggerInput = Input::GetRightControllerTrigger(0);
	if (triggerInput > 0.0f)
	{
		playerComponent->currentPower += playerComponent->chargeSpeed * deltaTime * triggerInput;
	}
	else if (Input::IsKeyHeld(pcComponent->increasePowerKC))
	{
		playerComponent->currentPower += playerComponent->chargeSpeed * deltaTime;
	}
	playerComponent->currentPower = std::min(100.0f, playerComponent->currentPower);
}

void PCSystem::DecreasePower(const float deltaTime, PlayerComponent* playerComponent, PCComponent* pcComponent)
{
	const float triggerInput = Input::GetLeftControllerTrigger(0);
	if (triggerInput > 0.0f)
	{
		playerComponent->currentPower -= playerComponent->chargeSpeed * deltaTime * triggerInput;
	}
	else if (Input::IsKeyHeld(pcComponent->increasePowerKC))
	{
		playerComponent->currentPower -= playerComponent->chargeSpeed * deltaTime;
	}
	playerComponent->currentPower = std::min(100.0f, playerComponent->currentPower);
}

void PCSystem::Fire(ECS::Registry& registry, const ECS::EntityHandle& handle, PlayerComponent* playerComponent, PCComponent* pcComponent)
{
	// No charge so skip firing check
	if (playerComponent->currentPower <= 0.0f || !playerComponent->canFire || playerComponent->levelComplete)
	{
		return;
	}

	if (Input::IsKeyDown(pcComponent->fireProjectileKC) || Input::IsControllerKeyDown(pcComponent->fireProjectileCC))
	{
		MeshComponent* arrowMesh = registry.GetComponent<MeshComponent>(playerComponent->arrowMeshHandle);
		if (arrowMesh)
		{
			arrowMesh->isVisible = false;
		}

		TransformComponent* playerTransform = registry.GetComponent<TransformComponent>(playerComponent->projectileHandle);

		if (VelocityComponent* velocityComponent = registry.GetComponent<VelocityComponent>(handle))
		{
			velocityComponent->velocity = ProjectileMotion::CalculateInitalVelocity(playerComponent->currentPower * 0.33f, playerTransform->forwardVector);
		}

		if (RigidBodyComponent* playerRigidBody = registry.GetComponent<RigidBodyComponent>(handle))
		{
			playerRigidBody->affectedByGravity = true;
		}

		SetArrowMeshVisibility(registry, playerComponent, false);
		playerComponent->canFire = false;
		// Reset charge amount, could keep it previous amount of set it to default like 50
		playerComponent->lastLocation = playerTransform->worldPosition;
		playerComponent->strokesUsed++;
	}
}

void PCSystem::Look(const float deltaTime, ECS::Registry& registry, PlayerComponent* playerComponent, PCComponent* pcComponent)
{
	AnalogInput input = Input::GetLeftControllerAnaloge(0);

	float keyInputX = Input::IsKeyHeld(pcComponent->lookLeftKC) - Input::IsKeyHeld(pcComponent->lookRightKC);
	float keyInputY = Input::IsKeyHeld(pcComponent->lookUpKC) - Input::IsKeyHeld(pcComponent->lookDownKC);

	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(playerComponent->cameraSpringArm))
	{
		BMath::Quaternion quatX = BMath::Quaternion::Identity();
		BMath::Quaternion quatY = BMath::Quaternion::Identity();

		CameraComponent* cameraComponent = CameraHelper::GetMainCamera(registry);
		if (!cameraComponent)
		{
			LOGMESSAGE(Error, "Error finding main camera component");
			return;
		}

		if (input.x != 0.0f)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(-input.x), BMath::Vector3(cameraComponent->upVector));
		}
		else if (keyInputX)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(keyInputX), BMath::Vector3(cameraComponent->upVector));
		}

		if (input.y != 0.0f)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(input.y), BMath::Vector3(cameraComponent->rightVector));
		}
		else if (keyInputY)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(keyInputY), BMath::Vector3(cameraComponent->rightVector));
		}

		parentRotationComponent->quat = quatY * quatX;
		parentRotationComponent->isAdditive = true;
	}
}

void PCSystem::Aim(const float deltaTime, ECS::Registry& registry, PlayerComponent* playerComponent, PCComponent* pcComponent)
{
	AnalogInput input = Input::GetRightControllerAnaloge(0);

	float keyInputX = Input::IsKeyHeld(pcComponent->aimRightKC) - Input::IsKeyHeld(pcComponent->aimLeftKC);
	float keyInputY = Input::IsKeyHeld(pcComponent->aimDownKC) - Input::IsKeyHeld(pcComponent->aimUpKC);

	if (Behemoth::RotationComponent* parentRotationComponent = registry.GetComponent<Behemoth::RotationComponent>(playerComponent->projectileHandle))
	{
		BMath::Quaternion quatX = BMath::Quaternion::Identity();
		BMath::Quaternion quatY = BMath::Quaternion::Identity();

		TransformComponent* projectileTransform = registry.GetComponent<TransformComponent>(playerComponent->projectileHandle);

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
			quatY = BMath::Quaternion(DEGREE_TO_RAD(input.y), projectileTransform->rightVector);
		}
		else if (keyInputY != 0.0f)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(keyInputY), projectileTransform->rightVector);
		}

		parentRotationComponent->quat = (quatX * quatY).Normalize();
		parentRotationComponent->isAdditive = true;
	}
}


void PCSystem::SetArrowMeshVisibility(ECS::Registry& registry, PlayerComponent* playerComponent, bool isVisible)
{
	MeshComponent* arrowMeshComponent = registry.GetComponent<MeshComponent>(playerComponent->arrowMeshHandle);
	if (!arrowMeshComponent)
	{
		LOGMESSAGE(Error, "Unable to get arrow mesh component");
		return;
	}
	
	arrowMeshComponent->isVisible = isVisible;
}

void PCSystem::RotateMeshWhileMoving(ECS::Registry& registry, const ECS::EntityHandle& entity, PlayerComponent* playerComponent)
{
	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(playerComponent->playerMeshHandle))
	{
		if (VelocityComponent* velocity = registry.GetComponent<VelocityComponent>(entity))
		{
			float speed = velocity->velocity.Magnitude();
			BMath::Vector3 dir = BMath::Vector3::Normalize(BMath::Vector3(-velocity->velocity.z, 0.0f, -velocity->velocity.x));
			parentRotationComponent->quat = BMath::Quaternion(DEGREE_TO_RAD(5.0f), dir);
			parentRotationComponent->isAdditive = true;
		}
	}
}

void PCSystem::CheckPlayerLanded(ECS::Registry& registry, const ECS::EntityHandle& entity, PlayerComponent* playerComponent)
{
	CollisionDataComponent* collisionData = registry.GetComponent<CollisionDataComponent>(entity);
	if (!collisionData)
	{
		// Not colliding with ground so exit 
		return;
	}
	// bool isCollidingWithGround = false;
	if (VelocityComponent* velocity = registry.GetComponent<VelocityComponent>(entity))
	{

// 		for (const auto& data : collisionData->data)
// 		{
// 			if (data.data.collisionNormal == BMath::Vector3::Up())
// 			{
// 				isCollidingWithGround = true;
// 				break;
// 			}
// 		}

		if (BMath::Vector3::SquaredMagnitude(velocity->velocity) <=  1.0f)
		{
			velocity->velocity = BMath::Vector3(0.0f);
			playerComponent->canFire = true;
		}
	}
}