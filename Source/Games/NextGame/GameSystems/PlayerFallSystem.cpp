#include "pch.h"
#include "PlayerFallSystem.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/Components.h"

void PlayerFallSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [
		entityHandle, 
			playerComp, 
			playerFellComp] : registry.Get<PlayerComponent, PlayerFallComponent>())
	{
 		if (Behemoth::TransformComponent* springArmTransform = registry.GetComponent<Behemoth::TransformComponent>(playerComp->cameraSpringArm))
 		{
 			springArmTransform->localPosition = springArmTransform->worldPosition;
 			springArmTransform->localTransform._41 = springArmTransform->localPosition.x;
 			springArmTransform->localTransform._42 = springArmTransform->localPosition.y;
 			springArmTransform->localTransform._43 = springArmTransform->localPosition.z;
 		}

		playerComp->isRespawning = true;
		registry.RemoveComponent<Behemoth::ChildComponent>(playerComp->cameraSpringArm);

		ECS::EntityHandle timerEntity = registry.CreateEntity("Timer Component");
		registry.AddComponent<Behemoth::TimerComponent>(timerEntity, playerFellComp->timeUntilRespawn,
			[this, &registry, entityHandle]() { this->RespawnPlayer(registry, entityHandle); });

		registry.RemoveComponent<PlayerFallComponent>(entityHandle);
	}
}

void PlayerFallSystem::RespawnPlayer(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
{
	registry.AddComponent<PlayerRespawnComponent>(entityHandle);
}
