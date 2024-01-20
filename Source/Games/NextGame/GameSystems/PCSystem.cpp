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
	for (const auto& [
				   entity,
					   playerComp,
			pcComponent,
			velocityComp]: registry.Get<PlayerComponent, PCComponent, VelocityComponent>())
	{
		IncreasePower(deltaTime, playerComp, pcComponent);
		DecreasePower(deltaTime, playerComp, pcComponent);
		Look(deltaTime, registry, playerComp, pcComponent);
		Aim(deltaTime, registry, playerComp, pcComponent);

		Fire(registry, entity, playerComp, pcComponent);

		SwapCamera(pcComponent);
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
	if (playerComponent->currentPower <= 0.0f)
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
			velocityComponent->velocity = ProjectileMotion::CalculateInitalVelocity(playerComponent->currentPower, playerTransform->forwardVector);
		}

		if (RigidBodyComponent* playerRigidBody = registry.GetComponent<RigidBodyComponent>(handle))
		{
			playerRigidBody->affectedByGravity = true;
		}

		// Reset charge amount, could keep it previous amount of set it to default like 50
		playerComponent->currentPower = 0.0f;
	}
}

void PCSystem::Look(const float deltaTime, ECS::Registry& registry, PlayerComponent* playerComponent, PCComponent* pcComponent)
{
	AnalogInput input = Input::GetLeftControllerAnaloge(0);

	// No look input
	if (input == AnalogInput())
	{
		return;
	}

	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(playerComponent->cameraSpringArm))
	{
		BMath::Quaternion quatX = BMath::Quaternion::Identity();
		BMath::Quaternion quatY = BMath::Quaternion::Identity();

		TransformComponent* springArmTransform = registry.GetComponent<TransformComponent>(playerComponent->cameraSpringArm);

		if (input.x != 0.0f)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(input.x), BMath::Vector3(springArmTransform->upVector));
		}

		if (input.y != 0.0f)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(-input.y), BMath::Vector3(springArmTransform->rightVector));
		}

		parentRotationComponent->quat = quatY * quatX;
		parentRotationComponent->isAdditive = true;
	}
}

void PCSystem::Aim(const float deltaTime, ECS::Registry& registry, PlayerComponent* playerComponent, PCComponent* pcComponent)
{
	AnalogInput input = Input::GetRightControllerAnaloge(0);

	// No look input
	if (input == AnalogInput())
	{
		return;
	}

	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(playerComponent->projectileHandle))
	{
		BMath::Quaternion quatX = BMath::Quaternion::Identity();
		BMath::Quaternion quatY = BMath::Quaternion::Identity();

		TransformComponent* projectileTransform = registry.GetComponent<TransformComponent>(playerComponent->projectileHandle);

		if (input.x != 0.0f)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(input.x), projectileTransform->upVector);
		}

		if (input.y != 0.0f)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(-input.y), projectileTransform->rightVector);
		}

		parentRotationComponent->quat = (quatX * quatY).Normalize();

		parentRotationComponent->isAdditive = true;
	}
}


void PCSystem::SwapCamera(PCComponent* pcComponent)
{

}

void PCSystem::SetArrowMeshVisibility(ECS::Registry& registry, const ECS::EntityHandle& entity, bool isVisible)
{

}