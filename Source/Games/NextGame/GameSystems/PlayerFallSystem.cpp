#include "pch.h"
#include "PlayerFallSystem.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/Components.h"

void PlayerFallSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [
		entity, 
			playerComponent, 
			playerFellComponent] : registry.Get<PlayerComponent, PlayerFellComponent>())
	{
 		if (Behemoth::TransformComponent* springArmTransform = registry.GetComponent<Behemoth::TransformComponent>(playerComponent->cameraSpringArm))
 		{
 			springArmTransform->localPosition = springArmTransform->worldPosition;
 			springArmTransform->localTransform._41 = springArmTransform->localPosition.x;
 			springArmTransform->localTransform._42 = springArmTransform->localPosition.y;
 			springArmTransform->localTransform._43 = springArmTransform->localPosition.z;
 		}

		playerComponent->isRespawning = true;
		registry.RemoveComponent<Behemoth::ChildComponent>(playerComponent->cameraSpringArm);

		ECS::EntityHandle timerEntity = registry.CreateEntity("Timer Component");
		registry.AddComponent<Behemoth::TimerComponent>(timerEntity, playerFellComponent->timeUntilRespawn,
			[this, &registry, entity]() { this->RespawnPlayer(registry, entity); });

		// RespawnPlayer(registry, entity);
		registry.RemoveComponent<PlayerFellComponent>(entity);
	}
}

void PlayerFallSystem::RespawnPlayer(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
{
	registry.AddComponent<PlayerRespawnComponent>(entityHandle);
}
