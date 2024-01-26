#include "pch.h"
#include "PlayerRespawnSystem.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/Components.h"
#include "Factories/GameObjectFactory.h"
#include "Scripts/PlayerFactory.h"

void PlayerRespawnSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [
		entity,
			playerComponent,
			playerRespawnComponent,
			transformComponent,
			velocityComponent,
			rigidBodyComponent] 
		:
		registry.Get<PlayerComponent, PlayerRespawnComponent, Behemoth::TransformComponent, Behemoth::VelocityComponent, Behemoth::RigidBodyComponent>())
	{
		// Add necessary child component to respective parents in hierarchy
		Behemoth::GameObjectFactory::AddChildObject(registry, entity, playerComponent->cameraSpringArm);

		registry.AddComponent<Behemoth::MoveComponent>(entity, playerComponent->lastLocation, false);
	 	registry.AddComponent<Behemoth::MoveComponent>(playerComponent->cameraSpringArm, playerComponent->lastLocation, false);
		velocityComponent->velocity = BMath::Vector3::Zero();
		playerComponent->canFire = true;
		rigidBodyComponent->affectedByGravity = false;
		playerComponent->isRespawning = false;

		registry.RemoveComponent<PlayerRespawnComponent>(entity);
	}
}
