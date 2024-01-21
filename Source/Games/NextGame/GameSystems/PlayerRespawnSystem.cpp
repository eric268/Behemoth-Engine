#include "pch.h"
#include "PlayerRespawnSystem.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/Components.h"
#include "Factories/GameObjectFactory.h"
#include "Scripts/PlayerFactory.h"

using namespace Behemoth;

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
		registry.Get<PlayerComponent, PlayerRespawnComponent, TransformComponent, VelocityComponent, RigidBodyComponent>())
	{

		 GameObjectFactory gameObjectFactory{};
		// Add necessary child component to respective parents in hierarchy
		 gameObjectFactory.AddChildObject(registry, entity, playerComponent->cameraSpringArm);

		registry.AddComponent<MoveComponent>(entity, playerComponent->lastLocation, false);
	 	registry.AddComponent<MoveComponent>(playerComponent->cameraSpringArm, playerComponent->lastLocation, false);
		velocityComponent->velocity = BMath::Vector3::Zero();
		playerComponent->canFire = true;
		playerComponent->currentPower = 0.0f;
		rigidBodyComponent->affectedByGravity = false;


		registry.RemoveComponent<PlayerRespawnComponent>(entity);
	}
}
