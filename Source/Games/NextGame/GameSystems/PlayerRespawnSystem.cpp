#include "pch.h"
#include "PlayerRespawnSystem.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/Components.h"
#include "Factories/GameObjectFactory.h"
#include "Scripts/PlayerFactory.h"
#include "Misc/GameObjectHelper.h"

void PlayerRespawnSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [
		entityHandle,
			playerComp,
			playerRespawnComp,
			transformComp,
			velocityComp,
			rigidBodyComp] 
		: registry.Get<PlayerComponent, PlayerRespawnComponent, Behemoth::TransformComponent, Behemoth::VelocityComponent, Behemoth::RigidBodyComponent>())
	{
		// Add necessary child component to respective parents in hierarchy
		Behemoth::GameObjectHelper::AddChildObject(registry, entityHandle, playerComp->cameraSpringArm);

		registry.AddComponent<Behemoth::MoveComponent>(entityHandle, playerComp->lastLocation, false);
	 	registry.AddComponent<Behemoth::MoveComponent>(playerComp->cameraSpringArm, playerComp->lastLocation, false);
		velocityComp->velocity = BMath::Vector3::Zero();
		playerComp->canFire = true;
		rigidBodyComp->affectedByGravity = false;
		playerComp->isRespawning = false;

		registry.RemoveComponent<PlayerRespawnComponent>(entityHandle);
	}
}
